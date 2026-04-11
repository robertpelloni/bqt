package kernel

import (
	"fmt"
	"testing"
)

func TestUndoStackPushUndoAndHistoryLimit(t *testing.T) {
	var order []string
	us := &UndoStack{}

	us.Push(UndoAction{
		UserID:      "user-a",
		Description: "first",
		Undo: func() {
			order = append(order, "undo:first")
		},
	})
	us.Push(UndoAction{
		UserID:      "user-b",
		Description: "second",
		Undo: func() {
			order = append(order, "undo:second")
		},
	})
	us.Push(UndoAction{
		UserID:      "user-a",
		Description: "third",
		Undo: func() {
			order = append(order, "undo:third")
		},
	})

	us.Undo("user-a")
	if len(order) != 1 || order[0] != "undo:third" {
		t.Fatalf("expected most recent user-a action to be undone first, got %+v", order)
	}
	if len(us.actions) != 2 {
		t.Fatalf("expected 2 remaining actions after undo, got %d", len(us.actions))
	}
	if len(us.redoPool) != 1 || us.redoPool[0].Description != "third" {
		t.Fatalf("expected redo pool to contain undone action, got %+v", us.redoPool)
	}

	us.Undo("missing-user")
	if len(order) != 1 {
		t.Fatalf("expected missing-user undo to do nothing, got %+v", order)
	}

	limited := &UndoStack{}
	for i := 0; i < 501; i++ {
		idx := i
		limited.Push(UndoAction{
			UserID:      "user",
			Description: fmt.Sprintf("action-%03d", idx),
			Undo:        func() {},
		})
	}
	if len(limited.actions) != 500 {
		t.Fatalf("expected undo history to be capped at 500, got %d", len(limited.actions))
	}
	if limited.actions[0].Description != "action-001" {
		t.Fatalf("expected oldest retained action to be action-001, got %q", limited.actions[0].Description)
	}
}

func TestClipboardSetBinaryAndReceiveRemote(t *testing.T) {
	clip := GetClipboard()
	clip.SetText("hello")
	if clip.GetText() != "hello" {
		t.Fatalf("expected local text to be stored, got %q", clip.GetText())
	}

	blob := []byte{1, 2, 3, 4}
	clip.SetBinary(blob)
	got := clip.GetBinary()
	if len(got) != 4 || got[0] != 1 || got[3] != 4 {
		t.Fatalf("expected binary payload to round-trip, got %v", got)
	}

	clip.ReceiveRemote("remote-text", []byte{9, 8})
	if clip.GetText() != "remote-text" {
		t.Fatalf("expected remote text overwrite, got %q", clip.GetText())
	}
	if len(clip.GetBinary()) != 2 || clip.GetBinary()[0] != 9 || clip.GetBinary()[1] != 8 {
		t.Fatalf("expected remote binary overwrite, got %v", clip.GetBinary())
	}

	clip.ReceiveRemote("", nil)
	if clip.GetText() != "remote-text" {
		t.Fatalf("expected empty remote text to preserve existing text, got %q", clip.GetText())
	}
	if len(clip.GetBinary()) != 2 || clip.GetBinary()[0] != 9 || clip.GetBinary()[1] != 8 {
		t.Fatalf("expected nil remote binary to preserve existing binary, got %v", clip.GetBinary())
	}
}

func TestMergeKernelProcessEditAppendsHistory(t *testing.T) {
	mk := &MergeKernel{}
	op1 := EditOp{UserID: "user-a", Pos: 3, Text: "abc", Timestamp: 100}
	op2 := EditOp{UserID: "user-b", Pos: 5, Text: "xyz", Timestamp: 200}

	mk.ProcessEdit(op1)
	mk.ProcessEdit(op2)

	if len(mk.History) != 2 {
		t.Fatalf("expected 2 merged edits in history, got %d", len(mk.History))
	}
	if mk.History[0] != op1 {
		t.Fatalf("expected first edit to be preserved, got %+v", mk.History[0])
	}
	if mk.History[1] != op2 {
		t.Fatalf("expected second edit to be preserved, got %+v", mk.History[1])
	}
}
