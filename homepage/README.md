# 홈페이지
> 간략한 프로젝트 소개와 고객들의 서비스 이용 현황

### 개발 환경
- Python (3.5.1)
- Django
```
$ sudo pip install -r requirements.txt
```

### 실행 방법
- 가상 환경 구축
```
$ sudo pip install virtualenv
$ mkdir ~/work/pyenv
$ cd ~/work/pyenv
$ virtualenv --no-site-packages --distribute web
$ source web/bin/activate
```
- 개발 환경 구축
```
(web) $ sudo pip install -r requirements.txt
(web) $ python manage.py makemigrations
(web) $ python manage.py migrate
(web) $ python manage.py createsuperuser
Username : admin
...
(web) $ python manage.py runserver
```