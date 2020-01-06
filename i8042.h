#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#ifndef BIT
#define BIT(n) (0x01 << (n))
#endif

#define DELAY_US 20000         /* IBM's specification of the i8042 requires the keyboard to respond to a command in 20 ms */
#define KBD_IRQ_ADDRESS 0x09   /* KBD's address in the PIC vector */
#define MOUSE_IRQ_ADDRESS 0x74 /* Mouse's address in the PIC vector */
#define STAT_REG_PAR BIT(7)     /* Parity Error       - invalid data */
#define STAT_REG_TIMEOUT BIT(6) /* Timeout Error      - invalid data */
#define STAT_REG_AUX BIT(5)     /* Mouse Data */
#define STAT_REG_INH BIT(4)     /* Inhibit Flag       - if keyboard is inhibited */
#define STAT_REG_A2 BIT(3)      /* A2 Input Line      - 0 if data byte - 1 if command byte */
#define STAT_REG_SYS BIT(2)     /* System Flag        - 0 if system in power-on reset - 1 if system already initialized */
#define STAT_REG_IBF BIT(1)     /* Input Buffer Full  - do not write commands or arguments */
#define STAT_REG_OBF BIT(0)     /* Output Buffer Full - data available for reading */
#define KBC_READ_CMD 0x20  /* Returns Command Byte */
#define KBC_WRITE_CMD 0x60 /* Writes Command Byte */
#define KBC_WRITE_MOUSE_BYTE 0xD4
#define KBC_DIS_MOUSE_CMD 0xA7 /* Disables Mouse Interface */
#define KBC_ENA_MOUSE_CMD 0xA8 /* Enables Mouse Interface */
#define KBC_INT_MOUSE_CMD 0xA9 /* Checks Mouse Interface, returns: 0 if OK */
#define KBC_CHECK_CMD 0xAA /* Checks KBC (self-test), returns: 0x55 if OK, 0xFC if error */
#define KBC_INT_KBD_CMD 0xAB /* Checks KBD Interface, returns: 0 if OK */
#define KBC_DIS_KBD_CMD 0xAD /* Disables KBD Interface */
#define KBC_ENA_KBD_CMD 0xAE /* Enables KBD Interface */
#define KBC_CMD_BYTE_DIS2 BIT(5) /* Disables Mouse */
#define KBC_CMD_BYTE_DIS BIT(4)  /* Disables Keyboard Interface */
#define KBC_CMD_BYTE_INT2 BIT(1) /* Enables Interrupt on OBF, from mouse */
#define KBC_CMD_BYTE_INT BIT(0)  /* Enables Interrupt on OBF, from keyboard */
#define OUT_BUF 0x60  /* Output Buffer */
#define IN_BUF 0x60   /* Input Buffer */
#define STAT_REG 0x64 /* Status Register */
#define CTRL_REG 0x64 /* Control Register */
#define ESC_BREAKCODE 0x81
#define TWO_BYTE_SCANCODE 0xE0 /* Two-byte long scancodes usually use 0xE0 as their first byte */
#define MAKE_CODE BIT(7)
#define MOUSE_LB BIT(0)     /* Left Button Status */
#define MOUSE_RB BIT(1)     /* Right Button Status */
#define MOUSE_MB BIT(2)     /* Middle Button Status */
#define MOUSE_X_SIGN BIT(4) /* X Coordinate Sign - 0 for positive, 1 for negative */
#define MOUSE_Y_SIGN BIT(5) /* Y Coordinate Sign - 0 for positive, 1 for negative */
#define MOUSE_X_OVFL BIT(6) /* X Coordinate Overflow */
#define MOUSE_Y_OVFL BIT(7) /* Y Coordinate Overflow */
#define MOUSE_RESET 0xFF       /* Resets Mouse */
#define MOUSE_RESEND 0xFE      /* For serial communication errors */
#define MOUSE_DEFAULTS 0xF6    /* Set default values */       /* Disable Data Reporting - In stream mode, should be sent before any other command */
#define MOUSE_SAMPLE_RATE 0xF3 /* Sets state sampling rate */
#define MOUSE_REMOTE 0xF0      /* Sets remote mode - sends data on request only */
#define MOUSE_READ 0xEB        /* Sends data packet request */
#define MOUSE_STREAM 0xEA      /* Sets stream mode - sends data on events */
#define MOUSE_STATUS 0xE9      /* Gets mouse configuration (3 bytes) */
#define MOUSE_RESOLUTION 0xE8  /* Sets resolution */
#define MOUSE_SCAL2 0xE7       /* Scaling 2:1 - Acceleration mode */
#define MOUSE_SCAL1 0Xe6       /* Scaling 1:1 - Linear mode */
#define ACK 0xFA   /* Mouse acknowledgment if everything OK */
#define NACK 0xFE  /* Mouse acknowledgment if invalid byte */
#define ERROR 0xFC /* Mouse acknowledgment if second consecutive invalid byte */

#define UP 0x48 /* Up arrow */
#define DOWN 0x50 /* Down arrow */
#define RIGHT 0x4D /* Right arrow */
#define LEFT 0x4B /* Left arrow */
#define ENTER 0x1C /* Enter key */

#define OBF BIT(0)
#define AUX_ERR BIT(5)
#define PAR_ERR BIT(7)
#define TO_ERR BIT(6)

#define MOUSE_IRQ 12
#define LB 0x01
#define RB 0x02
#define MB 0x04
#define MSB_X 0x10
#define MSB_Y 0x20
#define OVF_X 0x40
#define OVF_Y 0x80

#define EN_DATA_RP 0xF4

#endif
