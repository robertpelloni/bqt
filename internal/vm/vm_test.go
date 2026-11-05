package vm

import (
	"strings"
	"testing"
)

func TestVMEvalAddition(t *testing.T) {
	vm := NewVM()
	if vm == nil {
		t.Fatal("Expected NewVM to return non-nil instance")
	}

	// Test literal evaluation
	lit, err := vm.Eval(42.0)
	if err != nil || lit != 42.0 {
		t.Fatalf("Expected 42.0, got %v (err: %v)", lit, err)
	}

	// Test string variable resolution
	vm.GlobalEnv.vars["foo"] = 100.0
	res, err := vm.Eval("foo")
	if err != nil || res != 100.0 {
		t.Fatalf("Expected 100.0, got %v (err: %v)", res, err)
	}

	// Test function call: (+ 10.0 20.0 30.0)
	expr := []interface{}{"+", 10.0, 20.0, 30.0}
	sum, err := vm.Eval(expr)
	if err != nil {
		t.Fatalf("Failed to eval (+ ...): %v", err)
	}
	if sum != 60.0 {
		t.Fatalf("Expected sum 60.0, got %v", sum)
	}
}

func TestMacroRecorder(t *testing.T) {
	mr := GetMacroRecorder()
	if mr == nil {
		t.Fatal("Expected GetMacroRecorder to return non-nil instance")
	}

	mr.IsRecording = true
	mr.CommandLog = make([]string, 0)

	mr.RecordClick("btn-save")
	mr.RecordClick("btn-run")

	script := mr.GenerateScript()
	if !strings.Contains(script, "(click \"btn-save\")") {
		t.Errorf("Expected script to contain (click \"btn-save\"), got %s", script)
	}
	if !strings.Contains(script, "(click \"btn-run\")") {
		t.Errorf("Expected script to contain (click \"btn-run\"), got %s", script)
	}

	mr.IsRecording = false
}

func TestNeuralEngineCaptureDOM(t *testing.T) {
	ne := GetNeuralEngine()
	if ne == nil {
		t.Fatal("Expected GetNeuralEngine to return non-nil instance")
	}

	domJSON, err := ne.CaptureDOM()
	if err != nil {
		t.Fatalf("CaptureDOM returned error: %v", err)
	}
	if !strings.Contains(domJSON, "BQt-Go") {
		t.Errorf("Expected DOM JSON to contain BQt-Go, got %s", domJSON)
	}
}

func TestAIAssistant(t *testing.T) {
	aa := GetAIAssistant()
	if aa == nil {
		t.Fatal("Expected GetAIAssistant to return non-nil instance")
	}

	s := AISuggestion{Text: "fmt.Println()", Confidence: 0.95}
	aa.SetLatestSuggestion(s)

	got := aa.GetLatestSuggestion()
	if got.Text != s.Text || got.Confidence != s.Confidence {
		t.Errorf("Expected suggestion %v, got %v", s, got)
	}

	aa.RequestCompletion("func testContext()")
}

func TestExecuteAIAction(t *testing.T) {
	ok := ExecuteAIAction("win-terminal", "focus")
	if !ok {
		t.Errorf("Expected ExecuteAIAction to return true")
	}
}
