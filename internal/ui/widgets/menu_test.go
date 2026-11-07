package widgets

import (
	"image"
	"testing"

	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/unit"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

func TestMenuLayoutAndSelection(t *testing.T) {
	menu := &Menu{
		Visible: false,
		Items: []*MenuItem{
			{Label: "File", Action: "file"},
			{Label: "Edit", Action: "edit"},
			{Label: "Exit", Action: "exit"},
		},
		X:       10,
		Y:       20,
		WidthDp: 0, // Fallback to 200
	}

	var ops op.Ops
	gtx := layout.Context{
		Ops: &ops,
		Constraints: layout.Exact(image.Point{
			X: 800,
			Y: 600,
		}),
		Metric: unit.Metric{PxPerDp: 1.0, PxPerSp: 1.0},
	}

	th := theme.GetTheme(theme.Dark)

	// Hidden menu returns empty dims and -1 index
	dimsHidden, idxHidden := menu.Layout(gtx, th)
	if dimsHidden.Size.X != 0 || dimsHidden.Size.Y != 0 || idxHidden != -1 {
		t.Fatalf("expected empty dims and -1 index for hidden menu, got %+v, idx=%d", dimsHidden.Size, idxHidden)
	}

	// Visible menu renders and width falls back to 200
	menu.Visible = true
	dims, idx := menu.Layout(gtx, th)
	if idx != -1 {
		t.Fatalf("expected -1 when no item clicked, got %d", idx)
	}
	if menu.WidthDp != 200 {
		t.Fatalf("expected fallback width 200, got %f", menu.WidthDp)
	}
	if dims.Size.X != 800 || dims.Size.Y != 600 {
		t.Fatalf("expected full constraint dimensions, got %+v", dims.Size)
	}

	// Trigger click on item index 1 ("Edit")
	menu.Items[1].Click.Click()
	_, clickedIdx := menu.Layout(gtx, th)
	if clickedIdx != 1 {
		t.Fatalf("expected selected index 1, got %d", clickedIdx)
	}
	if menu.Visible {
		t.Fatal("expected menu.Visible to be false after item click")
	}

	// Bounds clamping test
	menu.Visible = true
	menu.X = 750 // X + Width (200) > 800, will clamp
	menu.Y = 550 // Y + Height > 600, will clamp
	menu.Layout(gtx, th)
}
