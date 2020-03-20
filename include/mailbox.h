#ifndef _MAILBOX_H_
#define _MAILBOX_H_
#include "RayOSDef.h"
void MailSend(ray_mailbox_t *mailbox, ray_uint32_t mail);
void MailRecieve(ray_mailbox_t *mailbox, ray_uint32_t *mail);
#endif
