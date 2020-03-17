// Based on https://github.com/newaetech/chipwhisperer.git

#include "simpleserial.h"
#include <stdint.h>
#include "hal-stm32f4.h"

typedef struct ss_cmd
{
	char c;
	unsigned int len;
	uint8_t (*fp)(int, uint8_t*);
} ss_cmd;

static ss_cmd commands[MAX_SS_CMDS];
static int num_commands = 0;

static char hex_lookup[16] =
{
	'0', '1', '2', '3', '4', '5', '6', '7',
	'8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

static int hex_decode(int len, char* ascii_buf, uint8_t* data_buf)
{
	for(int i = 0; i < len; i++)
	{
		char n_hi = ascii_buf[2*i];
		char n_lo = ascii_buf[2*i+1];

		if(n_lo >= '0' && n_lo <= '9')
			data_buf[i] = n_lo - '0';
		else if(n_lo >= 'A' && n_lo <= 'F')
			data_buf[i] = n_lo - 'A' + 10;
		else if(n_lo >= 'a' && n_lo <= 'f')
			data_buf[i] = n_lo - 'a' + 10;
		else
			return 1;

		if(n_hi >= '0' && n_hi <= '9')
			data_buf[i] |= (n_hi - '0') << 4;
		else if(n_hi >= 'A' && n_hi <= 'F')
			data_buf[i] |= (n_hi - 'A' + 10) << 4;
		else if(n_hi >= 'a' && n_hi <= 'f')
			data_buf[i] |= (n_hi - 'a' + 10) << 4;
		else
			return 1;
	}

	return 0;
}

// Callback function for "v" command.
// This can exist in v1.0 as long as we don't actually send back an ack ("z")
static uint8_t check_version(int size, uint8_t* v)
{
	return 0x00;
}

// Set up the SimpleSerial module by preparing internal commands
// This just adds the "v" command for now...
void simpleserial_init()
{
	simpleserial_addcmd('v', 0, check_version);
}

int simpleserial_addcmd(char c, unsigned int len, uint8_t (*fp)(int, uint8_t*))
{
	if(num_commands >= MAX_SS_CMDS)
		return 1;

	if(len >= MAX_SS_LEN)
		return 1;

	commands[num_commands].c   = c;
	commands[num_commands].len = len;
	commands[num_commands].fp  = fp;
	num_commands++;

	return 0;
}

int simpleserial_get(void)
{
	char ascii_buf[2*MAX_SS_LEN];
	char c;

	// Find which command we're receiving
	c = hal_recv();

	int cmd;
	for(cmd = 0; cmd < num_commands; cmd++)
	{
		if(commands[cmd].c == c)
			break;
	}

	// If we didn't find a match, give up right away
	if(cmd == num_commands)
		return GET_ERROR;

	// Receive characters until we fill the ASCII buffer
  unsigned int i;
  int end = 0;
	for(i = 0; i < 2*commands[cmd].len; i++)
	{
		c = hal_recv();

		// Check for early \n
		if(c == '\n' || c == '\r'){
      end = 1;
			break;
    }

		ascii_buf[i] = c;
	}

  // Check for even number of uint16_t
  if(i % 2 != 0)
    return GET_ERROR;

	// Assert that last character is \n or \r
  if(end == 0){
    c = hal_recv();
    if(c != '\n' && c != '\r')
      return GET_ERROR;
  }

	// ASCII buffer is full: convert to bytes 
	// Check for illegal characters here
	uint8_t data_buf[i/2];
	if(hex_decode(i/2, ascii_buf, data_buf))
		return GET_ERROR;

	// Callback
	uint8_t ret;
	ret = commands[cmd].fp(i/2, data_buf);
	
	// Acknowledge (if version is 1.1)
	simpleserial_put('z', 1, &ret);

  return i/2;
}

void simpleserial_put(char c, int size, uint8_t* output)
{
	// Write first character
	hal_send_char(c);

	// Write each byte as two nibbles
	for(int i = 0; i < size; i++)
	{
		hal_send_char((char) hex_lookup[output[i] >> 4 ]);
		hal_send_char((char) hex_lookup[output[i] & 0xF]);
	}

	// Write trailing '\n'
	hal_send_char('\n');
}
