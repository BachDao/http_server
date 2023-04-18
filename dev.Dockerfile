# Remove docker enviroment for development purpose
#
# Build and run:
#   docker build -t alpile-cpp:1.0 -f dev.Dockerfile .
#   docker run -d --cap-add sys_ptrace --privileged -p127.0.0.1:2222:22 -p 127.0.0.1:8080:8080 alpile-cpp:1.0
#   ssh-keygen -f "$HOME/.ssh/known_hosts" -R "[localhost]:2222"
#
# stop:
#   docker stop clion_remote_env
#
# ssh credentials (test user):
#   root@root

FROM alpine:latest
RUN apk add --no-cache cmake --repository=http://dl-cdn.alpinelinux.org/alpine/edge/main

RUN apk add --update openssh gcc g++ clang gdb cmake  ninja autoconf automake dos2unix tar rsync python3 git\
          build-base \
          curl \
          pkgconfig \
          unzip \
          zip \
          lighttpd \
          openrc \
  && rm  -rf /tmp.cpp/* /var/cache/apk/* \


RUN echo "root:root" | chpasswd

RUN ( \
	echo 'Port 22'; \
	echo '#AddressFamily any'; \
	echo '#ListenAddress 0.0.0.0'; \
	echo '#ListenAddress ::'; \
	echo 'HostKey /etc/ssh/ssh_host_rsa_key'; \
	echo 'HostKey /etc/ssh/ssh_host_dsa_key'; \
	echo 'HostKey /etc/ssh/ssh_host_ecdsa_key'; \
	echo 'HostKey /etc/ssh/ssh_host_ed25519_key'; \
	echo 'PermitRootLogin yes'; \
	echo '#PasswordAuthentication yes'; \
	echo 'AuthorizedKeysFile .ssh/authorized_keys'; \
	echo 'Subsystem sftp /usr/lib/ssh/sftp-server'; \
  ) > /etc/sshd_config_test_clion

RUN /usr/bin/ssh-keygen -A
RUN ssh-keygen -t rsa -b 4096 -f  /etc/ssh/ssh_host_key

# http test server
RUN rc-update add lighttpd default
RUN echo "Lighttpd is running..." > /var/www/localhost/htdocs/index.html


EXPOSE 22
EXPOSE 80
CMD ["/usr/sbin/sshd", "-D", "-f", "/etc/sshd_config_test_clion"]