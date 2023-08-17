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
# sed -rn '/^BO_.*BDCU/{s/ (\w*):/ ch0_tx_\1:/g; s/$/\n=========\n/g; p;}' ${source0}


sed -rn '
/^BO_/{ 							# 先找BO_开头的行A
	/BDCU/!{ 						# 如果行A不含有BDCU字符串
		:b; h; n;{ 					# 将数据覆盖存入hold空间，读取下一行A+1
			:c;				
			/^$/b; 					# A+1为空时，退出本次循环，否则继续
			/^BO_.*BDCU/b; 			# A+1含有BO和BDCU时，退出本次循环，否则继续
			/^BO_/bb; 				# A+1含有BO,不含BDCU,跳转到:b
			/\bSG_.*BDCU/!{			# A+1不含有目标数据SG BDCU，则
				 H; n; bc;			# 向hold空间追加当前A+1行数据，读取下一行，跳转到:c
			}						# A+1行数据含有目标数据SG BDCU
			H; g; 					# 则向hold空间追加当前A+1行数据，并从hold空间获取全部数据
			s/ (\w*):/ =====_\1:/; 
			s/$/\n=========\n/g; 
			p;
		}
	}
}
' ${source0}
