hayes.h :		config.h clock.h \
		cmos.h hardware.h v40init.h led.h watchdog.h delay.h \
		8255.h ppi.h lbx_pio.h icu.h tcu.h scu.h rtc.h uart.h rom.h \
		analog.h remtron.h cmd_tmio.h \
		rdeb.h rlc.h status.h lpm_sbx.h \
		asm.h \
		protocal.h checksum.h mon_i_or.h mon_puts.h cmdtmbuf.h \
		hostbuf.h 8250.h \
		dspio.h \
		hayeslib.mak
			del hayes.h
			del temp.h
			copy config.h+clock.h temp.h
			copy temp.h+cmos.h+hardware.h+v40init.h+led.h+watchdog.h+delay.h hayes.h
			del temp.h
			copy hayes.h+8255.h+ppi.h+lbx_pio.h+icu.h+tcu.h+scu.h+rtc.h+uart.h+rom.h temp.h
			copy temp.h+analog.h+remtron.h+cmd_tmio.h hayes.h
			del temp.h
			copy hayes.h+rdeb.h+rlc.h+status.h+lpm_sbx.h+asm.h temp.h
			copy temp.h hayes.h
			copy hayes.h+protocal.h+checksum.h+mon_i_or.h+mon_puts.h+cmdtmbuf.h temp.h
			copy temp.h hayes.h
			copy hayes.h+hostbuf.h+8250.h+dspio.h+mit_io.h temp.h
			copy temp.h hayes.h
			del temp.h
