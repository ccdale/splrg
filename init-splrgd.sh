#!/bin/sh
### BEGIN INIT INFO
# Provides:             splrg
# Required-Start:       $network $named $remote_fs $syslog
# Required-Stop:        $network $named $remote_fs $syslog
# Default-Start:        2 3 4 5
# Default-Stop:         0 1 6
# Short-Description:    start the splrgd service at boot time
# Description:          Enables the splrgd daemon which responds on port 2208 to run puppet
### END INIT INFO

PIDDIR=/var/tmp
PIDFILE=${PIDDIR}/splrgd.pid

PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin

export PATH

# see if the wrapper script is there
test -x /usr/bin/splrgd || exit 0

. /lib/lsb/init-functions

case $1 in
    status)
        if [ -r $PIDFILE ]; then
            read pid <$PIDFILE
            if [ -n ${pid:-} ]; then
                if ps h $pid >/dev/null; then
                    log_daemon_msg "splrgd is running"
                    enum=0;
                else
                    log_daemon_msg "splrgd is not running but pidfile exists."
                    enum=1;
                fi
            else
                log_daemon_msg "splrgd Server is not running."
                enum=0;
            fi
        else
            log_daemon_msg "splrgd Server is not running."
            enum=0;
        fi
        log_end_msg $enum
        ;;
    restart|force-reload)
        $0 stop
        sleep 1
        $0 start
        ;;
    stop)
        log_daemon_msg "Stopping splrgd Server" splrgd
        start-stop-daemon --stop --quiet --pidfile $PIDFILE
        pid=$(ps auxw |grep [s]plrgd|sed 's/[ \t]\+/ /'|cut -d" " -f2)
        if [ -n "${pid:-}" ]; then
            # failed to stop
            log_end_msg 1
        else
            log_end_msg 0
        fi
        ;;
    start)
        log_daemon_msg "Starting splrgd Server" bgch-service
        if ! start-stop-daemon --start --quiet --oknodo --exec /usr/bin/splrgd
        then
            log_end_msg 1
            exit 1
        fi
        log_end_msg 0
        ;;
esac

