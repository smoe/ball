### list all filenames of the directory here ###
SET(GROUP DOCKING/IMGDOCK)

FILE(GLOB HEADERS_LIST "include/BALL/${GROUP}/*.h" "include/BALL/${GROUP}/*.iC")

ADD_BALL_HEADERS("${GROUP}" "${HEADERS_LIST}")