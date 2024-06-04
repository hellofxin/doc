echo "======================================================================"

# 输入DBC
sourceDbc=$1
echo "sourceDbc: ${sourceDbc}"
if [ ! -e ${sourceDbc} ]; then
	echo "source dbc is not specified"
	echo "exit"
	exit
fi
echo "sourceDbc: ${sourceDbc}"

# 
sourceNode=$2
echo "sourceNode: ${sourceNode}"
if [ -z ${sourceNode} ]; then
	echo "source node is not specified"
	echo "0 is used"
	sourceNode='0'
fi
echo "sourceNode: ${sourceNode}"

targetDbc=${sourceNode}_${sourceDbc}
echo "targetDbc: ${targetDbc}"
if [ -e ${targetDbc} ]; then
	# rm $targetDbc
	mv ${targetDbc} "${targetDbc}.bak"
fi
touch $targetDbc


prefixTx="CH${sourceNode}_TX_"
echo "prefixTx: ${prefixTx}"

prefixRx="CH${sourceNode}_RX_"
echo "prefixRx: ${prefixRx}"

prefixModified="TO_BE_MODIFIED::"
echo "prefixModified: ${prefixModified}"

prefixDeleted="TO_BE_DELETED::"
echo "prefixDeleted: ${prefixDeleted}"

NodeName="HCU"


file_rxtx_msg="${sourceNode}_rxtx_msg.txt"
if [ -e $file_rxtx_msg ]; then
	rm $file_rxtx_msg
fi
touch $file_rxtx_msg

file_msgAttribute="${sourceNode}_msgAttribute.txt"
if [ -e $file_msgAttribute ]; then
	rm $file_msgAttribute
fi
touch $file_msgAttribute


sed -En "
	/^BO_ /{
		/${NodeName}/!{
			# /${prefixRx}/!{
				s/ (ch._.x_)?(\w+):/ ${prefixRx}\2:/g;
				h; 
				:a; n; /SG_/{
					/${NodeName}/!ba;
					s/: [0-9]+\|[0-9]+@/: 0\|64@/g;
					H; g; p; b; 
				}
			# }
		}
	}
" ${sourceDbc} >> ${file_rxtx_msg}


echo "" >> ${file_rxtx_msg}
echo "" >> ${file_rxtx_msg}


sed -En "
	/^BO_ .*${NodeName}/{
		# /${prefixTx}/!{
			# s/(.*)/${prefixModified}\1/;
			s/ (ch._.x_)?(\w+):/ ${prefixTx}\2:/g;
			p;
			
			n; 
			/SG_/!b;
			# s/(.*)/${prefixModified}\1/;
			s/: [0-9]+\|[0-9]+@/: 0\|64@/g;
			p;
			
			:a; n; /SG_/{
				# s/(.*)/${prefixDeleted}\1/;
				# p;
				# d;
				# ba;
			} 
		# }
	}
" ${sourceDbc} >> ${file_rxtx_msg}


# cat ${rx_bo} >> ${txt_rx_tx_bo}
# cat ${tx_bo} >> ${txt_rx_tx_bo}


cat ${file_rxtx_msg} | while read target_line; do
	echo "======================================================================================================"
	echo "target_line: ${target_line}"
	target_line_id=`echo  ${target_line} | sed -E "/BO_ /s/(BO_ )([0-9]+)( \w+: .*)/\2/"`
	echo "target_line_id: ${target_line_id}"

	if [ "${target_line}" != "${target_line_id}" ]; then
		echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
		echo "FOUND MSG: ${target_line_id}"
		sed -En "
		/BA_ .* BO_/{
			/${target_line_id}/{
				p;
			}
		}
		" ${sourceDbc} >> ${file_msgAttribute}
	else
		echo "#########################################################################################"
		echo "skip signal"
		echo ""
	fi
done


sed -En "
	/^$/p;
	/^VERSION/p;
	/^NS_/p;
	/^\W+\w+_$/p;
	/^BS_/p;
	/^BU_/p;
" ${sourceDbc} >> ${targetDbc}

cat ${file_rxtx_msg} >> ${targetDbc}

sed -En "	
	/^$/p;
	/^BA_DEF_ /p;
	/^BA_DEF_DEF_ /p;
	/^BA_ .DBName./p;
	/^BA_ .BusType./p;
" ${sourceDbc} >> ${targetDbc}

cat ${file_msgAttribute} >> ${targetDbc}

# remove 中间文件
rm $file_rxtx_msg
rm $file_msgAttribute
