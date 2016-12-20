Set iInter = WScript.CreateObject("Trushin.InterpolationInclusion")
Dim MyArray
MyArray = Array(1, 2, 3, 4, 5, 6, 7)
iInter.PushInputFromArray(MyArray)
iInter.PerformInterpolationTriple
Dim units
units = iInter.GetResultTripleOutputUnits
WScript.Echo "units: " & units

Dim str
str = ""

For i = 0 to units - 1
	For j = 0 to units - 1
		For k = 0 to units - 1
			str = str & iInter.GetResultTripleElement(i, j, k) & "   "
		Next
		str = str & vbNewLine
	Next
	str = str & vbNewLine
	str = str & vbNewLine
Next

WScript.Echo str