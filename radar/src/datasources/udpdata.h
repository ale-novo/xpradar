/* This is the udpdata.h header to the udpdata.c code

   Copyright (C) 2009 - 2014  Reto Stockli

   This program is free software: you can redistribute it and/or modify it under the 
   terms of the GNU General Public License as published by the Free Software Foundation, 
   either version 3 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
   without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
   See the GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along with this program.  
   If not, see <http://www.gnu.org/licenses/>. 
*/

extern int udpSendBufferLen;
extern int udpRecvBufferLen;
extern char *udpSendBuffer;
extern char *udpRecvBuffer;
extern int udpReadLeft;                      /* counter of bytes to read from receive thread */

/* prototype functions */
void allocate_udpdata(int sendlen,int recvlen);
void get_udpdata(char *data, int len);
void deallocate_udpdata();
