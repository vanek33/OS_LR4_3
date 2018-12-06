#define PUBLIC "public" /* имя общедоступного именованного программного канала */
#define LINESIZE 512

struct message
{
	char privfifo[15]; /* имя частного именованного канала */
	char filename[100]; /* имя запрошенного файла */
};
