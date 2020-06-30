
// 当心字节对齐的问题
typedef struct
{
  uint8_t head;
  uint8_t cmd;
  uint16_t length;
  uint8_t data[1];
} package_t;

 
static int parse_temperature(char *buffer)
{
  int value = *(int *)buffer;
  printf("temperature = %d\n", value);
}

static int parse_humidity(char *buffer)
{
  int value = *(int *)buffer;
  printf("humidity = %d\n", value);
}

static int parse_illumination(char *buffer)
{
  int value = *(int *)buffer;
  printf("illumination = %d\n", value);
}

 
typedef struct
{
  uint8_t cmd;
  void (* handle)(char *buffer);
} package_entry_t;

 
static const package_entry_t package_items[] =
{
  {0x01, parse_temperature},
  {0x02, parse_humidity},
  {0x03, parse_illumination},
  {0xFF, NULL},
};

 
static uint8_t parse(char *buffer, uint16_t length)
{
  package_t *frame = (package_t *)buffer;
  uint16_t crc = CRCCheck(buffer, length - 3);
  uint8_t tail = buffer[length - 1]; 
  const package_entry_t *entry;

  if((frame->head != xxx) && (tail != xxx) && (crc != (buffer[length - 3]) << 8 | buffer[length - 2]))
  {
    return 0;
  }

 
  for(entry = package_items; entry->handle != NULL; ++entry)
  {

    if(frame->cmd == entry->cmd)
    {
      entry->handle(frame->data);
      break;
    }
  }
  return 1;
}
