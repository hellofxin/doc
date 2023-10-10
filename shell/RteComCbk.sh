comcallback="ComCallback.c"

sourceFile=./src/autosarConfig/Rte_COMCbk.c 
# cp ${sourceFile} "${sourceFile}.bak"
sed -rn '/boolean ComIPduCallout/{:end; N; /}/!bend; /return/b; s/}/\treturn TRUE\;\n}\n/g; p}' ${sourceFile} >> ${comcallback}

cat ${comcallback}