package widgets

import (
	"image"
	"testing"

	"gioui.org/layout"
	"gioui.org/op"
)

func TestRHINode_Layout_ZeroHandle(t *testing.T) {
	node := &RHINode{
		NativeTextureHandle: 0,
	}

	ops := new(op.Ops)
	gtx := layout.Context{
		Ops:         ops,
		Constraints: layout.Exact(image.Point{X: 800, Y: 600}),
	}

	dims := node.Layout(gtx)
	if dims.Size.X != 0 || dims.Size.Y != 0 {
		t.Errorf("expected empty dims for zero texture handle, got %v", dims.Size)
	}
}

func TestRHINode_Layout_ValidHandle(t *testing.T) {
	node := &RHINode{
		NativeTextureHandle: uintptr(0x12345678),
	}

	ops := new(op.Ops)
	gtx := layout.Context{
		Ops:         ops,
		Constraints: layout.Exact(image.Point{X: 800, Y: 600}),
	}

	dims := node.Layout(gtx)
	if dims.Size.X != 800 || dims.Size.Y != 600 {
		t.Errorf("expected dims 800x600, got %v", dims.Size)
	}
}
