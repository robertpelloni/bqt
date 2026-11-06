package widgets

import (
	"image"
	"testing"

	"gioui.org/layout"
	"gioui.org/op"
)

func TestBobQBox_PackingAndLayout(t *testing.T) {
	box := NewBobQBox(layout.Horizontal, 5, false)
	if box == nil {
		t.Fatal("Expected NewBobQBox to return non-nil instance")
	}

	dummyWidget := func(gtx layout.Context) layout.Dimensions {
		return layout.Dimensions{Size: image.Pt(50, 30)}
	}

	// Pack widgets at start and end
	box.PackStart(dummyWidget, false, false, 2)
	box.PackEnd(dummyWidget, true, true, 2)

	var ops op.Ops
	gtx := layout.Context{
		Ops: &ops,
		Constraints: layout.Exact(image.Pt(300, 100)),
	}

	dims := box.Layout(gtx)
	if dims.Size.X == 0 || dims.Size.Y == 0 {
		t.Errorf("Expected non-zero dimensions from BobQBox.Layout, got %v", dims)
	}
}
