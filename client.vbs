Set iInter = WScript.CreateObject("Trushin.Interpolation")
Dim MyArray
MyArray = Array(1, 2, 3, 4, 5)
iInter.PushInputFromArray(MyArray)
iInter.PerformInterpolationBCube

Dim OutArray

If iInter.GetOutputLineFirst Then
	OutArray = iInter.OutputLine
	For i = LBound(OutArray) To UBound(OutArray)
		WScript.StdOut.Write OutArray(i) & " "
	Next
	WScript.Echo ""

	While iInter.GetOutputLineNext
		OutArray = iInter.OutputLine
		For i = LBound(OutArray) To UBound(OutArray)
			WScript.StdOut.Write OutArray(i) & " "
		Next
		WScript.Echo ""
	Wend
End If