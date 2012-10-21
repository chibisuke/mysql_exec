mysql_exec
==========

mysql_exec is a User defined function that allows executing of system commands on linux. 
I didn't test this one on BSD but it should work aswell. 

Keep in mind that in mysql UDFs cannot have ANY access permissions assigned to them. 
Anyonce gaining access to your mysql server can use it, to execute commands he likes with the
permission of the mysqld. 

For obvious reasons you should also avoid executing commands that won't terminate at some point.

Compiling
=========
sorry, no Makefile (yet). 
use the following command: 
> gcc -shared -fPIC -o udf_exec.so `mysql_config --include` udf_exec.c

on a 32bit system, you might want to remove the -fPIC


Installing
==========
after compiling it, you need to copy the .so file to your plugin directory.
The command "show variables like 'plugin_dir';" can be used to determin where your plugins
are surposed to be located.

After the file has been copied, you need to register the UDF with mysql
> CREATE FUNCTION exec RETURNS INTEGER SONAME 'udf_exec.so';

Using
=====
Just execute it as part of a normal SQL Query.

Example: 
SELECT exec('/bin/sh', '/usr/scripts/clear_cache.sh');

Unresolved Restrictions
=======================
exec() always required two parameter, where the first one is an executable and the second one
is a single argument. You cannot pass multiple arguments, and you cannot pass no argument.
The simplest way to cope with this restrictions is to put whatever you want to do in a wrapper
script, and then execute it. 

Keep in mind that I wrote this because I needed it myself, and never had any public usage in mind
I'm just releaseing it so other ppl can use it. 
Yes this means absolut NO WARANTY of any kind. If you break your server, don't blame me.

Boring Legal Stuff (2 clause BSD license)
=========================================

Copyright (c) 2012, Rene Henzinger
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice, 
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice, 
  this list of conditions and the following disclaimer in the documentation 
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR 
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON 
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
