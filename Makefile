PROG=proj

SRCS = proj.c graphicscard.c keyboard.c sprite.c timer.c utils.c levels.c collisions.c mouse.c levels1.c rtc.c levels2.c

# __LCOM_OPTIMIZED__ disables most of the LCF's overhead (and functionality!)
CPPFLAGS += -pedantic #-D __LCOM_OPTIMIZED__

DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>
