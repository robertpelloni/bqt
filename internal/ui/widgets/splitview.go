package widgets

import (
	"gioui.org/layout"
)

type SplitView struct {
	Ratio float32 // 0.0 to 1.0 split point
}

func (s *SplitView) Layout(gtx layout.Context, first, second layout.Widget) layout.Dimensions {
	// --- GO NATIVE GPU SPLITVIEW ---
	// High-performance partitioning natively in Go.
	if s.Ratio == 0 { s.Ratio = 0.5 }
	
	return layout.Flex{Axis: layout.Horizontal}.Layout(gtx,
		layout.Flexed(s.Ratio, first),
		layout.Rigid(func(gtx layout.Context) layout.Dimensions {
			// Draw Splitter Handle
			return layout.Dimensions{Size: gtx.Constraints.Min}
		}),
		layout.Flexed(1.0-s.Ratio, second),
	)
}
