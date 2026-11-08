package ui

import (
	"testing"

	"gioui.org/op"
)

func TestComputeKernel_DispatchAndSync(t *testing.T) {
	kernel := &ComputeKernel{
		Active: false,
	}

	ops := new(op.Ops)
	// When inactive, DispatchCompute returns early
	kernel.DispatchCompute(ops, "test_shader_code")

	// Activate kernel
	kernel.Active = true
	kernel.DispatchCompute(ops, "test_shader_code")

	buffer := kernel.SyncBuffer()
	if buffer == nil {
		t.Fatalf("expected non-nil buffer slice from SyncBuffer")
	}
	if len(buffer) != 0 {
		t.Errorf("expected empty buffer slice, got len %d", len(buffer))
	}
}
