echo "======================================================================"

sourceDbc=$1
echo "sourceDbc: ${sourceDbc}"
if [ ! -e ${sourceDbc} ]; then
	echo "source dbc is not specified"
	echo "exit"
	exit
fi

sourceNode=$2
echo "sourceNode: ${sourceNode}"
if [ -z ${sourceNode} ]; then
	echo "source node is not specified"
	echo "0 is used"
	${sourceNode}='0'
fi

prefixTx="ch${sourceNode}_tx_"
echo "prefixTx: ${prefixTx}"

prefixRx="ch${sourceNode}_rx_"
echo "prefixRx: ${prefixRx}"

rx_bo="rx_bo.txt"
if [ -e $rx_bo ]; then
	rm $rx_bo
fi
touch $rx_bo


# awk "/^BO_/,/BDCU/{print}" ${source1}

# OK awk
# awk '/^BO_/{gsub($3, "ch0"$3, $3); print;}' ${source0}

# OK sed
sed -ri.bak "/^BO_.*BDCU/{
	/${prefixTx}/!{
		s/ (ch._.x_)?(\w+):/ ${prefixTx}\2:/g;
		# p;
	}
}" ${sourceDbc}


sed -rn "
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
			H; g;					# 则向hold空间追加当前A+1行数据，并从hold空间获取全部数据
			/BO_ [0-9]+ \w+: [0-9]+ \w+/{
				# 此时读取了多行，写入的话，会将模式空间的多行写入当前LC位置，不对。
				# p;
				s/(BO_ [0-9]+ \w+: [0-9]+ \w+).*/\1/g;
				p;
			}
		}
	}
}
" ${sourceDbc} >> ${rx_bo}


cat ${rx_bo} | while read target_line; do
	echo "target_line: ${target_line}"
	sed -ri "
		/${target_line}/{
			# p;
			s/(BO_ [0-9]+ )(ch._.x_)?(\w+)(: [0-9]+ \w+)/\1${prefixRx}\3\4/g;
			# p;
		}
	" ${sourceDbc}
done


# cat ${source2} | while read src_line; do
# 	cat ${rx_bo} | while read target_line; do
# 		if [ ${src_line} == ${target_line} ]; then
# 			echo ${src_line}
# 		fi
# 	done
# done