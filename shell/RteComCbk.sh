sourceFile=./Rte_COMCbk.c 
sed -rn '/boolean ComIPduCallout/{:end; N; /}/!bend; s/}/\treturn TRUE\;\n}/g; p}' ${sourceFile}