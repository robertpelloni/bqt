package widgets

import (
	"image"
	"testing"

	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/unit"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

func TestQuickControlsLayout(t *testing.T) {
	var ops op.Ops
	gtx := layout.Context{
		Ops: &ops,
		Constraints: layout.Exact(image.Point{
			X: 400,
			Y: 300,
		}),
		Metric: unit.Metric{PxPerDp: 1.0, PxPerSp: 1.0},
	}

	th := theme.GetTheme(theme.Dark)

	// Test ProgressBar clamping and rendering
	pb := &ProgressBar{Value: -0.5}
	dimsPB := pb.Layout(gtx, th)
	if pb.Value != 0 {
		t.Fatalf("expected clamped value 0, got %f", pb.Value)
	}
	if dimsPB.Size.X != 400 || dimsPB.Size.Y != 10 {
		t.Fatalf("expected dims 400x10, got %+v", dimsPB.Size)
	}

	pb.Value = 1.5
	pb.Layout(gtx, th)
	if pb.Value != 1 {
		t.Fatalf("expected clamped value 1, got %f", pb.Value)
	}

	// Test Dial layout
	dial := &Dial{Value: 0.75}
	dimsDial := dial.Layout(gtx, th)
	if dimsDial.Size.X != 60 || dimsDial.Size.Y != 60 {
		t.Fatalf("expected dial dims 60x60, got %+v", dimsDial.Size)
	}

	// Test CheckBox layout
	cb := &CheckBox{Checked: false, Label: "Enable Feature"}
	dimsCB := cb.Layout(gtx, th)
	if dimsCB.Size.X != 20 || dimsCB.Size.Y != 20 {
		t.Fatalf("expected checkbox dims 20x20, got %+v", dimsCB.Size)
	}

	cb.Checked = true
	dimsCBChecked := cb.Layout(gtx, th)
	if dimsCBChecked.Size.X != 20 || dimsCBChecked.Size.Y != 20 {
		t.Fatalf("expected checked checkbox dims 20x20, got %+v", dimsCBChecked.Size)
	}
}
