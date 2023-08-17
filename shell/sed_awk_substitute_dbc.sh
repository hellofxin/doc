# BO_ 2565865010 ch0_tx_J1939_NM_TX: 8 BDCU
#  SG_ SIGNAL0 : 0|32@1+ (1,0) [0|65535] "" Vector__XXX
#  SG_ SIGNAL0 : 32|32@1+ (1,0) [0|65535] "" Vector__XXX
# BO_ 2565865010 ch0_tx_J1939_NM_1: 8 BDCU
#  SG_ SIGNAL0 : 0|32@1+ (1,0) [0|65535] "" Vector__XXX
#  SG_ SIGNAL0 : 32|32@1+ (1,0) [0|65535] "" Vector__XXX
# BO_ 2565865010 ch0_tx_J1939_NM_2: 8 CIOM
#  SG_ SIGNAL0 : 0|32@1+ (1,0) [0|65535] "" Vector__XXX
#  SG_ SIGNAL0 : 32|32@1+ (1,0) [0|65535] "" Vector__XXX
# BO_ 2565865214 ch0_rx_J1939_NM_RX: 8 Vector__XXX
#  SG_ SIGNAL0_11 : 0|32@1+ (1,0) [0|65535] ""  CIOM
#  SG_ SIGNAL0_12 : 32|32@1+ (1,0) [0|65535] ""  BDCU,CIOM
# BO_ 2565865214 ch0_rx_J1939_NM_1: 8 Vector__XXX
# SG_ SIGNAL0_12 : 32|32@1+ (1,0) [0|65535] ""  BDCU,CIOM
#  SG_ SIGNAL0_11 : 0|32@1+ (1,0) [0|65535] ""  CIOM
# BO_ 2565865214 ch0_rx_J1939_NM_2: 8 Vector__XXX
#  SG_ SIGNAL0_11 : 0|32@1+ (1,0) [0|65535] ""  CIOM
#  SG_ SIGNAL0_11 : 32|32@1+ (1,0) [0|65535] ""  RDCM


source0="BDCU_GCAN_v2.2_GenDBC20221013_test.dbc"
source1="BDCU_GCAN_v2.2_GenDBC20221013.dbc"
source2="00_BDCU_BCAN_GenDBC20230726.dbc"

# sed -nr ':a; N; /\bTIME/!ba; /RFTIME/!p;' list.txt
# awk "/^BO_/,/BDCU/{print}" ${source1}
# sed -rn '/^BO_/{:next; N; /\bSG_.*BDCU/!bnext; s/$/\n=========\n/g; p;}' ${source0}

# OK awk
# awk '/^BO_/{gsub($3, "ch0"$3, $3); print;}' ${source0}
# OK sed
sed -rn '/^BO_/{s/ (\w*):/ ch0_\1:/g; s/$/\n=========\n/g; p;}' ${source0}
