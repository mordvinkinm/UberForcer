#ifndef PROTOCOL_H
#define PROTOCOL_H

#define MSG_SEND_JOB                \
  "<?xml version=\"1.0\"?>\n"       \
  "<msg>\n"                         \
  " <type>MSG_SEND_JOB</type>\n"    \
  " <args>\n"                       \
  "   <job>\n"                      \
  "     <password>%s </password>\n" \
  "     <hash>%s </hash>\n"         \
  "     <alphabet>%s </alphabet>\n" \
  "     <from>%d</from>\n"          \
  "     <to>%d</to>\n"              \
  "   </job>\n"                     \
  " </args>\n"                      \
  "</msg>"

#define MSG_REPORT_RESULT                      \
  "<?xml version=\"1.0\"?>\n"                  \
  "<msg>\n"                                    \
  " <type>MSG_REPORT_RESULTS</type>\n"         \
  " <args>\n"                                  \
  "   <result>\n"                              \
  "     <password>%s </password>\n"            \
  "     <password_found>%d</password_found>\n" \
  "   </result>\n"                             \
  " </args>\n"                                 \
  "</msg>\n"

#endif /* PROTOCOL_H */
