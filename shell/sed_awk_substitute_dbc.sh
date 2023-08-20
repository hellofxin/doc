echo "======================================================================"

sourceDbc=$1
echo "sourceDbc: ${sourceDbc}"
if [ ! -e ${sourceDbc} ]; then
	echo "source dbc is not specified"
	echo "exit"
	exit
fi
echo "sourceDbc: ${sourceDbc}"

sourceNode=$2
echo "sourceNode: ${sourceNode}"
if [ -z ${sourceNode} ]; then
	echo "source node is not specified"
	echo "0 is used"
	sourceNode='0'
fi
echo "sourceNode: ${sourceNode}"

prefixTx="ch${sourceNode}_tx_"
echo "prefixTx: ${prefixTx}"

prefixRx="ch${sourceNode}_rx_"
echo "prefixRx: ${prefixRx}"

prefixModified="TO_BE_MODIFIED::"
echo "prefixModified: ${prefixModified}"

prefixDeleted="TO_BE_DELETED::"
echo "prefixDeleted: ${prefixDeleted}"

rx_bo="rx_bo.txt"
if [ -e $rx_bo ]; then
	rm $rx_bo
fi
touch $rx_bo

tx_bo="tx_bo.txt"
if [ -e $tx_bo ]; then
	rm $tx_bo
fi
touch $tx_bo


# awk "/^BO_/,/BDCU/{print}" ${source1}

# OK awk
# awk '/^BO_/{gsub($3, "ch0"$3, $3); print;}' ${source0}

echo "get tx bo"
# OK sed
sed -En "/^BO_.*BDCU/{
	/${prefixTx}/!{
		s/(.*)/${prefixModified}\1/g;
		p;
		# s/ (ch._.x_)?(\w+):/ ${prefixTx}\2:/g;
	}

	:a;
		n; 
		/SG_/!b;
		s/(.*)/${prefixModified}\1/;
		p;
		# s/: [0-9]+\|[0-9]+@/: 0\|64@/g;
		:c;
			n; 
			/SG_/!b;
			s/(.*)/${prefixDeleted}\1/g;
			p;
			# d;
			bc;
}" ${sourceDbc} >> ${tx_bo}


echo "rename tx bo"
cat ${tx_bo} | while read target_line; do
	echo ""
	echo "target_line: ${target_line}"
	target_line_header=`echo  ${target_line} | sed -E "/TO_BE_/s/(TO_BE_\w+::)(.*)/\1/"`
	target_line_body=`echo  ${target_line}   | sed -E "/TO_BE_/s/(TO_BE_\w+::)(.*)/\2/"`
	echo "target_line_header: ${target_line_header}"
	echo "target_line_body: ${target_line_body}"

	if [ "${prefixModified}" = ${target_line_header} ]; then
		sed -Ei "
		/${target_line_body}/{
			/BO_/{
				# p;
				s/(BO_ [0-9]+ )(ch._.x_)?(\w+)(: [0-9]+ \w+)/\1${prefixTx}\3\4/g;
				# p;
			}
			/SG_/{
				# p;
				s/: [0-9]+\|[0-9]+@/: 0\|64@/g;
				# p;
			}
		}
		" ${sourceDbc}	
	elif [ "${prefixDeleted}" = ${target_line_header} ]; then
		target_line_body=`echo  ${target_line_body} | sed -E "s/(.*SG\w+ )(\w+)( : .*)/\2/"`
		echo "target_line_body: ${target_line_body}"
		sed -Ei "
		/${target_line_body}/{
				# p;
				/BA_DEF/!d;
				# p;
		}
		" ${sourceDbc}
	else
		echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
		echo "not matched"
	fi
done



echo "get rx bo"
sed -En "
/^BO_/{ 							# 先找BO_开头的行A
	/BDCU/!{ 						# 如果行A不含有BDCU字符串

			:b; h; n;{ 					# 将数据覆盖存入hold空间，读取下一行A+1
				:c;				
				/^$/b; 					# A+1为空时，退出本次循环，否则继续
				/^BO_.*BDCU/b; 			# A+1含有BO和BDCU时，退出本次循环，否则继续
				/^BO_/bb; 				# A+1含有BO,不含BDCU,跳转到:b
				/\bSG_.*BDCU/!{			# A+1不含有目标数据SG BDCU，则
					 s/(.*)/${prefixDeleted}\1/g;
					 p;
					 H; n; bc;			# 向hold空间追加当前A+1行数据，读取下一行，跳转到:c
				}						# A+1行数据含有目标数据SG BDCU
				s/(.*)/${prefixModified}\1/;
				p; 
				H; g;					# 则向hold空间追加当前A+1行数据，并从hold空间获取全部数据
				/BO_ [0-9]+ \w+: [0-9]+ \w+/{
					# 此时读取了多行，写入的话，会将模式空间的多行写入当前LC位置，不对。
					# p;
					s/(BO_ [0-9]+ \w+: [0-9]+ \w+).*/${prefixModified}\1/g;
					p;
				}
				:cc;
					n; 
					/SG_/!b;
					s/(.*)/${prefixDeleted}\1/g;
					p;
					# d;
					bcc;
			}
	}
}
" ${sourceDbc} >> ${rx_bo}


echo "rename rx bo"
cat ${rx_bo} | while read target_line; do
	echo ""
	echo "target_line: ${target_line}"
	target_line_header=`echo  ${target_line} | sed -E "/TO_BE_/s/(TO_BE_\w+::)(.*)/\1/"`
	target_line_body=`echo  ${target_line}   | sed -E "/TO_BE_/s/(TO_BE_\w+::)(.*)/\2/"`
	echo "target_line_header: ${target_line_header}"
	echo "target_line_body: ${target_line_body}"

	if [ "${prefixModified}" = ${target_line_header} ]; then
		sed -Ei "
		/${target_line_body}/{
			/BO_/{
				# p;
				s/(BO_ [0-9]+ )(ch._.x_)?(\w+)(: [0-9]+ \w+)/\1${prefixRx}\3\4/g;
				# p;
			}
			/SG_/{
				# p;
				s/: [0-9]+\|[0-9]+@/: 0\|64@/g;
				# p;
			}
		}
		" ${sourceDbc}	
	elif [ "${prefixDeleted}" = ${target_line_header} ]; then
		target_line_body=`echo  ${target_line_body} | sed -E "s/(.*SG\w+ )(\w+)( : .*)/\2/"`
		echo "target_line_body: ${target_line_body}"
		sed -Ei "
		/${target_line_body}/{
				# p;
				/BA_DEF/!d;
				# p;
		}
		" ${sourceDbc}
	else
		echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
		echo "not matched"
	fi
done


# cat ${source2} | while read src_line; do
# 	cat ${rx_bo} | while read target_line; do
# 		if [ ${src_line} == ${target_line} ]; then
# 			echo ${src_line}
# 		fi
# 	done
# done


# sed  -Ei.bak "
# /BO_/{
# 	/BDCU/!{
# 		:a;
# 			n; /^$/b; /BO_/b; /SG_/{
# 				/BDCU/!{
# 					s/.*//g;
# 					# p;
# 					ba;
# 				}
# 			}
# 			s/: [0-9]+\|[0-9]+@/: 0\|64@/g;
# 			:c;
# 				n; /^$/b; /BO_/b; /SG_/{
# 					s/.*//g;
# 					# p;
# 					bc;
# 				}
# 	}
# }
# " ${sourceDbc}