#
# Copyright (c) 2018 [n/a] info@embeddora.com All rights reserved
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#        * Redistributions of source code must retain the above copyright
#          notice, this list of conditions and the following disclaimer.
#        * Redistributions in binary form must reproduce the above copyright
#          notice, this list of conditions and the following disclaimer in the
#          documentation and/or other materials provided with the distribution.
#        * Neither the name of The Linux Foundation nor
#          the names of its contributors may be used to endorse or promote
#          products derived from this software without specific prior written
#          permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NON-INFRINGEMENT ARE DISCLAIMED.    IN NO EVENT SHALL THE COPYRIGHT OWNER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

# Prefix for 'cross'es' (purposed for future) 
PREFIX=

# A suffix to tell 
SUFFIX=rlda



CFLAGS= -DWIDE_CONSOLE 		-DRESPONCE_LATENCY_GAUGING	-DRESPONCE_LATENCY_DISTRIBUTION_ANALYSIS          

CFLAGS += -DLEACKAGE_CHECK 	#-DHEAVY_DUTY_TEST

CFLAGS += -I/usr/local/include	-L/usr/local/lib      		-g -fprofile-arcs -ftest-coverage 

# -g debug symbols (to explore dumped core, for instance)

# -fprofile-arcs -ftest-coverage  (to collect code coverage statistics)

# -DEXTRA_INFORMATIVITY (to mix packet data and packet details in curl's output)

# -DEXTRA_PURITY (to clean intermediate files an completion; as the files contain useful site-crack hints they're valuable while debug/evaluation)

# -DEXTRA_PRECISION (to simulate conversation between browser and webpage _exactly by running not necessary HTTP-requests)

# -DRESPONCE_LATENCY_GAUGING (time computation) 

# -DLEACKAGE_CHECK (run few cycles to collect the extended statistics for valgrind-based memory profiling and others)

# -DHEAVY_DUTY_TEST (run extended set of HTTP-requests, collect more statisics data; useful for more precise analysis)



#  Code coverage statistics collecting:
#
#  REMOVE:gcov site_crawler.c
#
#  [optional]
#  1. gcovr -r . --xml-pretty -o ./site_crawler.rlg.xml  
#  1. firefox -url ./site_crawler.rlg.xml &
#
#  [mandatory]
#  2. gcovr -r . --html -o ./site_crawler.rlg.html -s
#  2. firefox -url ./site_crawler.rlg.html &
#
#  Command-line 'trafficing'
#
#  1. tcpdump -i eth0 'tcp port 80'
#  1. tcpdump -A -vvv -i eth0 src port 80
#
#  2. ngrep -q -d eth0 -W single host 192.168.0.1 and port 80
#
#  3. tcpdump -A -vvv -i eth0 src port 80 | strings
#
#  4. tcpdump -ni eth0 'port 80 and tcp[(tcp[12:1] & 0xf0>> 2):4] = 0x504f5354'
#
#  5. tcpdump -A -vvv -ni eth0 'port 80 and tcp[((tcp[12:1] & 0xf0) >> 2):4] = 0x504f5354' | grep 'POST'
#
#  6. tcpdump -s 0 -A 'tcp[((tcp[12:1] & 0xf0)>> 2):4] = 0x47455420'
#
#  man tcpdump | less -Ip examples
#
#  7. tcpdump -s 0 -A 'tcp[((tcp[12:1] & 0xf0)>> 2):4] = 0x47455420' | grep 'GET'
#
#  8. tcpdump -A -vvv -ni eth0 'tcp[((tcp[12:1] & 0xf0) >> 2):4] = 0x504f5354 or tcp[((tcp[12:1] & 0xf0)>> 2):4] = 0x47455420' | grep 'POST\|GET'




OBJS= site_crawler.$(SUFFIX).o structs.o site_drawer.o

GRBG=*.o *~ site_crawler.$(SUFFIX)


CC=$(PREFIX)gcc

LD=$(PREFIX)ld

all:	site_crawler.$(SUFFIX)

site_crawler.$(SUFFIX): $(OBJS)
	$(CC) $(CFLAGS)   $(OBJS)  -lcurl -lpng  -o $@

clean:
	rm $(GRBG) _delme.* *.gcn*  *.gcov  *.god*  *.gcd* site_crawler.xml site_crawler.html  valgrind.* callgrind.* massif.*  cachegrind.* *.html
