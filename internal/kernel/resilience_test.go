package kernel

import (
	"strings"
	"testing"
)

func TestResilience_ConsensusEngine(t *testing.T) {
	ce := GetConsensusEngine()
	if ce == nil {
		t.Fatalf("expected GetConsensusEngine to return non-nil instance")
	}

	if !ce.ProposeMutation("mut_dna_1") {
		t.Errorf("expected proposal to return true")
	}

	ce.HandlePeerVote("peer_node_1", "mut_dna_1", true)
	ce.HandlePeerVote("peer_node_2", "mut_dna_1", false)

	ce.Active = false
	if !ce.ProposeMutation("mut_dna_2") {
		t.Errorf("expected inactive proposal to return true")
	}
}

func TestResilience_GridCompute(t *testing.T) {
	jobData := []byte("compute_matrix_payload")
	DispatchJob("job_alpha", jobData)
	HandleRemoteTask("peer_cluster_1", "job_beta", jobData)
}

func TestResilience_SelfHealer(t *testing.T) {
	sh := GetSelfHealer()
	if sh == nil {
		t.Fatalf("expected GetSelfHealer to return non-nil instance")
	}

	var interceptedReport string
	aiCallback := func(report string) {
		interceptedReport = report
	}

	// Test intercepting an actual panic
	func() {
		defer sh.Intercept("faulty_task", aiCallback)
		panic("simulated fatal hardware fault")
	}()

	if interceptedReport == "" {
		t.Fatalf("expected panic report to be dispatched to aiCallback")
	}
	if !strings.Contains(interceptedReport, "simulated fatal hardware fault") {
		t.Errorf("expected report to contain panic message, got %q", interceptedReport)
	}
	if !strings.Contains(interceptedReport, "faulty_task") {
		t.Errorf("expected report to contain task name, got %q", interceptedReport)
	}

	// Inactive test
	sh.Active = false
	interceptedReport = ""
	func() {
		defer sh.Intercept("inactive_task", aiCallback)
		// Should return immediately without invoking callback when not panicked
	}()
	if interceptedReport != "" {
		t.Errorf("expected empty report when no panic occurred")
	}
}
