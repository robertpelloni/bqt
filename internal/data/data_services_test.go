package data

import (
	"testing"
)

func TestDataServices_GitKernel(t *testing.T) {
	gk := GetGitKernel()
	if gk == nil {
		t.Fatalf("expected GetGitKernel to return non-nil instance")
	}
	if gk.RepoPath == "" {
		t.Errorf("expected non-empty RepoPath, got %q", gk.RepoPath)
	}

	// Verify broadcast does not panic
	gk.BroadcastEvolution("1.2.22")
}

func TestDataServices_HistoryReconciler(t *testing.T) {
	hr := &HistoryReconciler{Active: true}
	hr.RequestMeshSync("/app/src/main.go")
	hr.MergeLedgers("peer_test_1", "/app/src/main.go", []string{"snap1", "snap2"})
}

func TestDataServices_DistributedDB(t *testing.T) {
	ddb := GetMeshDB()
	if ddb == nil {
		t.Fatalf("expected GetMeshDB to return non-nil instance")
	}

	// Connect in-memory DB if not yet connected
	if ddb.LocalDB.conn == nil {
		_ = ddb.LocalDB.Connect(":memory:")
	}

	// Initialize tables in database for safe execution
	_, err := ddb.LocalDB.ExecuteQuery("CREATE TABLE IF NOT EXISTS _test_dist (id INTEGER PRIMARY KEY, val TEXT);")
	if err != nil {
		t.Fatalf("failed to create test table: %v", err)
	}

	err = ddb.ExecuteReplicatedQuery("INSERT INTO _test_dist (val) VALUES ('mesh_val');")
	if err != nil {
		t.Fatalf("expected ExecuteReplicatedQuery to succeed, got %v", err)
	}

	ddb.HandleRemoteTransaction("peer_db_1", "INSERT INTO _test_dist (val) VALUES ('peer_val');")
}

func TestDataServices_PackageRegistry(t *testing.T) {
	reg := GetPackageRegistry()
	if reg == nil {
		t.Fatalf("expected GetPackageRegistry to return non-nil instance")
	}

	reg.FetchRemotePackage("peer_pkg_1", "bqt-audio-dsp")
	reg.HandleRemotePackage("peer_pkg_1", "bqt-audio-dsp", []byte{0x00, 0x01})
}

func TestDataServices_TimeMachine(t *testing.T) {
	tm := GetTimeMachine()
	if tm == nil {
		t.Fatalf("expected GetTimeMachine to return non-nil instance")
	}

	db := GetDatabase()
	if db.conn == nil {
		_ = db.Connect(":memory:")
	}
	if db.conn != nil {
		_, _ = db.conn.Exec("CREATE TABLE IF NOT EXISTS _omni_timemachine (id INTEGER PRIMARY KEY AUTOINCREMENT, filepath TEXT, content TEXT, timestamp DATETIME DEFAULT CURRENT_TIMESTAMP);")
	}

	err := tm.CommitState("/test/path.go", "package main\n")
	if err != nil {
		t.Fatalf("expected CommitState to succeed, got %v", err)
	}

	tm.ReceiveRemoteCommit("/test/remote.go", "package remote\n")
}
