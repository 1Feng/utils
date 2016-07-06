In ubuntu:
 sudo apt-get install liboost-python-dev
 sudo apt-get install python-dev

then:
 python setup.py build
 cd build/lib.linux-x86_****
 python
 >>> import hello
 >>> hello.greet()
