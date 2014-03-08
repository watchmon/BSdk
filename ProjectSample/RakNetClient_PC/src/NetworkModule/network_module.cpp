#include <unistd.h>
#include <stdlib.h>
#include "user_message_identify.h"
#include "user_common_define.h"
#include "network_module.h"
#include "RakNetSource/PacketLogger.h"

NETWORK_MODULE_BEGIN_NAMESPACE

Logger Network_module::ms_logger;

Network_module::Network_module()
{
    init_network_module();
}

Network_module::~Network_module()
{
    if (m_punch_proxy)
    {
        delete m_punch_proxy;
    }
}

void Network_module::run()
{
    print_config();

    using namespace RakNet;

    int l_addr_count = m_rakpeer->GetNumberOfAddresses();
    NET_MOD_INFO("获取到本机IP数量%d个", l_addr_count);

    for (int i = 0; i < l_addr_count; i++)
    {
        RakNet::SystemAddress addr = m_rakpeer->GetLocalIP(i);
        NET_MOD_INFO("第%d个IP地址为:%s", i + 1, addr.ToString(false));
    }

    RakNet::SocketDescriptor l_socket_descriptor;
    l_socket_descriptor.port = m_local_port;

    if (m_rakpeer->Startup(m_max_connection, &l_socket_descriptor, 1) != RakNet::RAKNET_STARTED)
    {
        NET_MOD_ERROR("启动网络引擎失败");
        RakNet::RakPeerInterface::DestroyInstance(m_rakpeer);
        return;
    }

    m_rakpeer->SetMaximumIncomingConnections(m_max_income_connection);

    NET_MOD_INFO("启动网络引擎成功，监控地址为%s", m_rakpeer->GetMyBoundAddress().ToString(false));

    NET_MOD_INFO("开始载入打孔插件");
    m_punch_proxy->init_proxy(m_rakpeer);

    if (m_rakpeer->Connect(DEFAULT_SERVER_IP, DEFAULT_SERVER_PORT, 0, 0) != RakNet::CONNECTION_ATTEMPT_STARTED)
    {
        NET_MOD_ERROR("网络引擎，尝试连接失败");
        return;
    }

    NET_MOD_INFO("尝试连接服务器IP地址为%s,端口为%d", DEFAULT_SERVER_IP, DEFAULT_SERVER_PORT);
    RakNet::Packet* packet = NULL;

    for (packet=m_rakpeer->Receive(); packet; m_rakpeer->DeallocatePacket(packet), packet=m_rakpeer->Receive())
    {
        if (packet->data[0]==ID_CONNECTION_REQUEST_ACCEPTED)
        {
            NET_MOD_INFO("连接服务器%s:%d成功", DEFAULT_SERVER_IP, DEFAULT_SERVER_PORT);
            break;
        }
        else if (packet->data[0]==ID_NO_FREE_INCOMING_CONNECTIONS)
        {
            NET_MOD_ERROR("服务器不在接受连接");
            return;
        }
        else
        {
            NET_MOD_ERROR("连接服务器失败");
            return;
        }
    }

    start_network_loop();
}

void Network_module::init_network_module()
{
    m_max_connection = DEFAULT_MAX_CONNECTION;
    m_max_income_connection = DEFAULT_MAX_INCOME_CONNECTION;
    m_local_port = DEFAULT_LOCAL_PORT;
    m_punch_proxy = new NatPunchProxy;
    m_stop_engine = false;
    m_rakpeer = RakNet::RakPeerInterface::GetInstance();
}

void Network_module::start_network_loop()
{
    using namespace RakNet;

    RakNet::Packet* l_packet = NULL;

    while (!m_stop_engine)
    {
        for (l_packet = m_rakpeer->Receive(); l_packet; m_rakpeer->DeallocatePacket(l_packet), l_packet = m_rakpeer->Receive())
        {
            process_packet(l_packet);
        }

        sleep(1);

        DataStructures::List<RakNet::SystemAddress> adds;
        DataStructures::List<RakNet::RakNetGUID> guids;
        m_rakpeer->GetSystemList(adds, guids);
        NET_MOD_INFO("%d已经连接", adds.Size());
    }

    if (m_punch_proxy)
    {
        m_punch_proxy->stop_proxy(m_rakpeer);
    }

    m_rakpeer->Shutdown(DEFAULT_SHUTDOWN_TIME);
    RakNet::RakPeerInterface::DestroyInstance(m_rakpeer);
}

void Network_module::process_packet(RakNet::Packet* p_packet)
{
    char* p_message_identify = (char*)RakNet::PacketLogger::BaseIDTOString(p_packet->data[0]);

    if (p_message_identify)
    {
        NET_MOD_DEBUG("接受消息，消息标志为%s", p_message_identify);
    }
    else
    {
        NET_MOD_DEBUG("接受消息，消息标志为%d", p_packet->data[0]);
    }
}

void Network_module::print_config() const
{
    NET_MOD_INFO("模块名为%s", get_name());
    NET_MOD_INFO("最大连接数为%d", m_max_connection);
    NET_MOD_INFO("最大被连接数为%d", m_max_income_connection);
    NET_MOD_INFO("本地监听端口为%d", m_local_port);
}

void Network_module::set_max_connection(const char* _max_connection)
{
    int max_connection = strtol(_max_connection, NULL, 10);
    m_max_connection = max_connection;
}

void Network_module::set_max_income_connection(const char* _max_income_connection)
{
    int max_income_connection = strtol(_max_income_connection, NULL, 10);
    m_max_income_connection = max_income_connection;
}

void Network_module::set_local_port(const char* _local_port)
{
    int local_port = strtol(_local_port, NULL, 10);
    m_local_port = local_port;
}

std::string Network_module::get_local_guid()
{
    std::string l_guid(m_rakpeer->GetMyGUID().ToString());
    return l_guid;
}

NatPunchProxy::NatPunchProxy():m_punch_client(NULL)
{
}

NatPunchProxy::~NatPunchProxy()
{
}

void NatPunchProxy::init_proxy(RakNet::RakPeerInterface* p_rakpeer)
{
    if (!p_rakpeer)
    {
        NET_MOD_ERROR("rakPeer为空指针，无法加载打孔插件");
        return;
    }

    m_punch_client = new RakNet::NatPunchthroughClient;
    p_rakpeer->AttachPlugin(m_punch_client);
    m_punch_client->SetDebugInterface(this);

    NET_MOD_INFO("打孔插件加载完毕");
}

void NatPunchProxy::stop_proxy(RakNet::RakPeerInterface* p_rakpeer)
{
    if (m_punch_client)
    {
        p_rakpeer->DetachPlugin(m_punch_client);
        delete m_punch_client;
    }

    m_punch_client = NULL;

    NET_MOD_INFO("打孔代理已停止");
}

NETWORK_MODULE_END_NAMESPACE
