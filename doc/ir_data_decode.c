
		int16_t val, data;
		unsigned char b;

	        for (i = 2; i < read_packet[1]; i+=2)
		{
		    val = (int16_t) ((unsigned short) read_packet[i + 0]) + (((unsigned short) read_packet[i + 1]) << 8);
	
		    if(val < 0)
		    {   /* SPACE */
		        data =  val;
		        printf("_SPACE_%d", (unsigned int) data);
		    }
		    else
		    {  /* PULSE */
		        data =  -((signed long) val);
		        data |= PULSE_BIT;
		        printf("_PULSE_%d", (unsigned int) data);
		    }
		}
