#/bin/bash

touch /tmp/f
exec 999<>/tmp/f
echo abc >&999
echo 123 >&999
cat /tmp/f
