package widgets

import (
	"image"
	"testing"

	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/unit"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

func TestTableViewLayout(t *testing.T) {
	tvEmpty := &TableView{}

	var ops op.Ops
	gtx := layout.Context{
		Ops: &ops,
		Constraints: layout.Exact(image.Point{
			X: 500,
			Y: 300,
		}),
		Metric: unit.Metric{PxPerDp: 1.0, PxPerSp: 1.0},
	}

	th := theme.GetTheme(theme.Dark)

	// Empty columns returns fallback label dimensions
	dimsEmpty := tvEmpty.Layout(gtx, th)
	if dimsEmpty.Size.X <= 0 || dimsEmpty.Size.Y <= 0 {
		t.Fatalf("expected non-zero fallback dimensions for empty table, got %+v", dimsEmpty.Size)
	}

	// Populated table
	tv := &TableView{
		Columns: []string{"ID", "Name", "Status"},
		Rows: [][]string{
			{"1", "Kernel", "Running"},
			{"2", "EventLoop", "Active"},
			{"3", "AudioGraph", "Streaming"},
		},
	}

	dims := tv.Layout(gtx, th)
	if dims.Size.X <= 0 || dims.Size.Y <= 0 {
		t.Fatalf("expected non-zero dimensions for populated table, got %+v", dims.Size)
	}
}

func TestTableView_tableMaxAndAlternatingRows(t *testing.T) {
	// 1. Test tableMax helper
	if tableMax(12, 6) != 12 {
		t.Errorf("expected 12, got %d", tableMax(12, 6))
	}
	if tableMax(3, 9) != 9 {
		t.Errorf("expected 9, got %d", tableMax(3, 9))
	}
	if tableMax(5, 5) != 5 {
		t.Errorf("expected 5, got %d", tableMax(5, 5))
	}

	// 2. Test table with alternating rows (odd/even)
	tv := &TableView{
		Columns: []string{"Key", "Value"},
		Rows: [][]string{
			{"Row 0", "Alpha"},
			{"Row 1", "Beta"},
			{"Row 2", "Gamma"},
			{"Row 3", "Delta"},
		},
	}

	var ops op.Ops
	gtx := layout.Context{
		Ops:         &ops,
		Constraints: layout.Exact(image.Point{X: 400, Y: 250}),
		Metric:      unit.Metric{PxPerDp: 1.0, PxPerSp: 1.0},
	}
	th := theme.GetTheme(theme.Cyberpunk)

	dims := tv.Layout(gtx, th)
	if dims.Size.X <= 0 || dims.Size.Y <= 0 {
		t.Fatalf("expected non-zero dimensions for alternating row layout, got %v", dims.Size)
	}
}
