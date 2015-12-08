/opt/apps/ansys/14.5/v145/fluent/bin/fluent-cleanup.pl it018 38701 CLEANUP_EXITING

LOCALHOST=`hostname`
if [ $LOCALHOST = it018 ]; then kill -9 31538; else ssh it018 kill -9 31538; fi
if [ $LOCALHOST = it018 ]; then kill -9 31537; else ssh it018 kill -9 31537; fi
if [ $LOCALHOST = it018 ]; then kill -9 31536; else ssh it018 kill -9 31536; fi
if [ $LOCALHOST = it018 ]; then kill -9 31535; else ssh it018 kill -9 31535; fi
if [ $LOCALHOST = it018 ]; then kill -9 31247; else ssh it018 kill -9 31247; fi
if [ $LOCALHOST = it018 ]; then kill -9 31081; else ssh it018 kill -9 31081; fi

rm -f /home/nathir2/ULTRA/batoid-burt-3/CFD/automation/setup/cleanup-fluent-it018-31247.sh
