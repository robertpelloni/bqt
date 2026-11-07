package widgets

import (
	"fmt"
	"image"
	"testing"

	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/unit"
	"github.com/robertpelloni/bqt/internal/kernel"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

func TestTerminalOutputAndLayout(t *testing.T) {
	proc := &kernel.Process{
		Output: make(chan string, 100),
	}

	term := &Terminal{
		Shell:  proc,
		Buffer: make([]string, 0),
	}

	// Send an output line
	proc.Output <- "system initialized: BQt microkernel active"

	var ops op.Ops
	gtx := layout.Context{
		Ops: &ops,
		Constraints: layout.Exact(image.Point{
			X: 600,
			Y: 400,
		}),
		Metric: unit.Metric{PxPerDp: 1.0, PxPerSp: 1.0},
	}

	th := theme.GetTheme(theme.Dark)

	dims := term.Layout(gtx, th)
	if len(term.Buffer) != 1 {
		t.Fatalf("expected buffer length 1, got %d", len(term.Buffer))
	}
	if term.Buffer[0] != "system initialized: BQt microkernel active" {
		t.Fatalf("unexpected buffer entry: %q", term.Buffer[0])
	}
	if dims.Size.X <= 0 || dims.Size.Y <= 0 {
		t.Fatalf("expected non-zero layout dimensions, got %+v", dims.Size)
	}

	// Test buffer limit of 50
	for i := 0; i < 60; i++ {
		term.Buffer = append(term.Buffer, fmt.Sprintf("log line %d", i))
	}
	// Trigger another read to force trimming
	proc.Output <- "overflow line"
	term.Layout(gtx, th)

	if len(term.Buffer) > 50 {
		t.Fatalf("expected buffer capped at 50, got %d", len(term.Buffer))
	}
}
