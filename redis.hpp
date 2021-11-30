#pragma once

#ifndef _REDIS_H_
#define _REDIS_H_

#include <iostream>
#include <string.h>
#include <string>
#include <stdio.h>
#include <queue>
#include <hiredis/hiredis.h>


class Redis {
public:
	Redis() {
                struct timeval timeout = {0, 200};
                this->_connect = (redisContext *)redisConnectWithTimeout("(host)", (port), timeout);
                if (this->_connect != NULL && this->_connect->err) {
                        printf("connect error: %s\n", this->_connect->errstr);
                }
                if ((NULL == this->_connect) || (this->_connect->err)){
                    if (this->_connect){
                        std::cout << "connect error:" << this->_connect->errstr << std::endl;
                    }else{
                        std::cout << "connect error: can't allocate redis context." << std::endl;
                    }
                }
                if (redisSetTimeout(this->_connect, timeout) != REDIS_OK){
                    std::cout <<"set timeout fail" << std::endl;
                }
                redisEnableKeepAlive(this->_connect);
        }

	~Redis() {
                redisFree(this->_connect);
		this->_connect = NULL;
		this->_reply = NULL;
                while(!m_clients.empty()) {  
                    redisContext *ctx = m_clients.front();  
                    redisFree(ctx);  
                    m_clients.pop();  
                 } 
	}
        
	bool connect(std::string host, int port) {
		this->_connect = redisConnect(host.c_str(), port);
		if (this->_connect != NULL && this->_connect->err) {
			printf("connect error: %s\n", this->_connect->errstr);
			return 0;
		}
		return 1;
	}

	std::string get(std::string key) {
                m_redisLock.lock();
                redisContext *ctx = CreateContext();
		this->_reply = (redisReply*)redisCommand(ctx, "GET %s", key.c_str());
                m_redisLock.unlock();
		std::string str = this->_reply->str;
                ReleaseContext(ctx, this->_reply != NULL);
                freeReplyObject(_reply);
		return str;
	}

        void close(std::string host, int port){
                redisFree(redisConnect(host.c_str(), port));
        }
    
    redisContext* CreateContext(){
        if (!m_clients.empty()){
            redisContext* ctx = m_clients.front();
            m_clients.pop();
            return ctx;
        }
        struct timeval tv = {2, 0};
        redisContext* ctx = redisConnectWithTimeout("(host)", (post), tv);
        if (ctx == NULL || ctx->err != 0){
            if (ctx != NULL) redisFree(ctx);
            return NULL;
        }

        return ctx;
    }

    void ReleaseContext(redisContext* ctx, bool active){
        if (ctx == NULL) return;
        if (!active) { redisFree(ctx); return; }
        m_clients.push(ctx);
    }


private:
	redisContext* _connect;
	redisReply* _reply;
        std::mutex m_redisLock;
        std::queue<redisContext *> m_clients;
};

#endif //_REDIS_H_
