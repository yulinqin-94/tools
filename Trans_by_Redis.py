import datetime, os, pdb, pexpect, sys, time, redis

# redis server
local_redis_ip = 'xxx.xxx.x.xxx'
local_redis_port = 

def push_redis(self,server,info):
    r = redis.Redis(host=local_redis_ip, port=local_redis_port, password="")
    key = 
    str_info = str(info)
    dict_redis = {key: str_info}
    r.set(key_server, str_info)
    
def get_redis(self,server):
    r = redis.Redis(host=local_redis_ip, port=local_redis_port, password="")
    res_bytes = r.get(key)
    res_str = res_bytes.decode("utf-8")
    res_dict = eval(res_str)
    return res_dict
