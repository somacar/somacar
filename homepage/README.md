# 홈페이지
> 간략한 프로젝트 소개와 고객들의 서비스 이용 현황

### 개발 환경
- CentOS ()
- Nginx
- Python (3.5.1)
- Django (1.9.6)

### 실행 방법
- Python 설치
```
$ sudo yum -y update
$ wget https://www.python.org/ftp/python/3.5.1/Python-3.5.1.tgz
$ tar zfx Python-3.5.1.tgz
$ cd Python-3.5.1
$ ./configure --prefix=/usr/local
$ sudo make altinstall
$ python3.5 --version
Python 3.5.1
```
- 가상 환경 구축
```
$ pip3.5 install virtualenv
$ mkdir ~/work/pyenv
$ cd ~/work/pyenv
$ virtualenv --no-site-packages --distribute web
```
- 개발 환경 구축
```
$ source ~/work/pyenv/web/bin/activate
(web) $ pip install -r requirements.txt     # permission error -> sudo chown -R $USER:$USER ~/work/pyenv
(web) $ python manage.py makemigrations
(web) $ python manage.py migrate
(web) $ python manage.py createsuperuser
Username : admin
...
```
- Nginx 설치
```
$ sudo yum -y install nginx
$ sudo vi /etc/nginx/conf.d/default.conf
    upstream app_server {
        server 127.0.0.1:8000 fail_timeout=0;
    }
    server {
        listen 80 default_server;
        listen [::]:80 default_server ipv6only=on;
        root /usr/share/nginx/html;
        index index.html index.htm;
        client_max_body_size 4G;
        server_name server.domain.com;
        keepalive_timeout 5;
        location /media  {
            alias /path/to/project/resources/media;
        }
        location /static {
            alias /path/to/project/resources/static;
        }
        location /static/admin {
            alias /usr/local/lib/python3.5/dist-packages/django/contrib/admin/static/admin;
        }
        location / {
            proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
            proxy_set_header Host $http_host;
            proxy_redirect off;
            proxy_pass http://app_server;
        }
    }
$ sudo service nginx restart
```
- 홈페이지 실행
```
$ source ~/work/pyenv/web/bin/activate
(web) $ python manage.py runserver
```