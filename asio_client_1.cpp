// asioClient.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#define BOOST_ASIO_DISABLE_STD_CHRONO
#define BOOST_CHRONO_HEADER_ONLY
#define BOOST_CHRONO_EXTENSIONS
#define BOOST_ASIO_ENABLE_HANDLER_TRACKING
#include <iostream>
#include <string>
#include <vector>
#include "nlohmann/json.hpp"
#include "boost/chrono.hpp"
#include <boost_1_74_0/boost/asio.hpp>
#include <boost_1_74_0/boost/asio/steady_timer.hpp>
#include <boost_1_74_0/boost/asio/system_timer.hpp>
#include <boost_1_74_0/boost/asio/high_resolution_timer.hpp>
#include <boost_1_74_0/boost/timer.hpp>
#include <boost_1_74_0/boost/bind.hpp>
#include <boost_1_74_0/boost/function.hpp>
#include <boost_1_74_0/boost/system/error_code.hpp>
#include <boost_1_74_0/boost/asio/ip/tcp.hpp>
using namespace boost::asio;
using namespace boost::chrono;
using namespace boost::system;
using namespace boost;
using json = nlohmann::json;
using  std::cin;
using std::cout;
using std::endl;
using std::vector;
using boost::shared_ptr;
seconds operator"" _s(unsigned long long n) {
	return seconds(n);
}

class timerWithFunc
{
private:
	int m_count = 0;
	int m_count_max = 0;
	function<void()> m_f;
	steady_timer m_t;
public:
	template<typename F>
	timerWithFunc(io_service& io, int x, F func) :m_count_max(x), m_f(func), m_t(io, 5_s) {
		init();
	}
private:
	typedef timerWithFunc this_type;
	void init() {
		m_t.async_wait(boost::bind(&this_type::handler, this, boost::asio::placeholders::error));
	}

	void handler(const error_code&)
	{
		m_f();
		//m_t.expires_from_now(milliseconds(1));
		//m_t.async_wait(bind(&this_type::handler, this, boost::asio::placeholders::error));
	}
};

class client {
	typedef client this_type;
	typedef ip::tcp::endpoint endpoint_type;
	typedef ip::address address_type;
	typedef ip::tcp::socket socket_type;
	typedef shared_ptr<socket_type> sock_ptr;
	typedef vector<char> buffer_type;

private:
	io_service	m_io;
	buffer_type		m_buf;
	endpoint_type	m_ep;
	steady_timer m_tm;
	vector<json> m_mess;
	json mess;
public:
	client() :
		m_buf(5, 0),
		m_ep(address_type::from_string("127.0.0.1"), 0x1234),
		m_tm(m_io, 10_s),
		mess(R"({"protocol": "keepalive"})"_json)
	{
		start();
	}

	void run()
	{
		m_io.run();
	}

	void start()
	{
		sock_ptr sock(new socket_type(m_io));
		//建立链接
		//sock->async_connect(m_ep,
		//	[this, sock](const error_code& ec)
		//	{
		//		if (ec)
		//		{
		//			cout << "connect error" << endl;
		//			return;
		//		}

		//		//异步发送接入消息，先发长度，发完后发送内容
		//		int length = mess.dump().length();
		//		sock->async_write_some(buffer("hello"),
		//			[&](const error_code& ec, std::size_t) {
		//				if (ec) {
		//					cout << "error occur,error code is	" << ec.value() << endl;
		//				}
		//				cout << "send length complete" << endl;
		//				//发送mess的内容
		//				send_handler(ec, sock);
		//			}
		//		);
		//	});

		sock->async_connect(m_ep, bind(&this_type::conn_handler, this, boost::asio::placeholders::error, sock));

		m_tm.async_wait(
			[](const error_code& ec) {
				cout << "timer bell" << endl;
			});
	}
	//发送内容，同时设定倒计时
	void send_handler(const error_code& ec, sock_ptr sock)
	{
		//cout << mess.dump() << endl;
		sock->async_write_some(buffer("jelakjdfg"),
			[&](const error_code& ec, std::size_t) {
				if (ec)
				{
					cout << "mess send error!" << ec.value() << "+" << ec.message() << endl;
				}
				cout << "mess send complete!" << endl;
			});

	}

	void conn_handler(const error_code& ec, sock_ptr sock)
	{
		if (ec)
		{
			return;
		}
		cout << "recive from " << endl;
		//int length = mess.length();
		sock->async_write_some(buffer("hello"),
			bind(&this_type::send_handler, this, boost::asio::placeholders::error, sock));

	}


};

int main()
{

	try
	{
		client c1;
		c1.run();
	}
	catch (const std::exception& e)
	{
		cout << e.what() << endl;
	}

}