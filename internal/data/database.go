package data

import (
	"database/sql"
	"log"
	_ "modernc.org/sqlite"
)

type Database struct {
	conn *sql.DB
}

var (
	dbInstance *Database
)

func GetDatabase() *Database {
	if dbInstance == nil {
		dbInstance = &Database{}
	}
	return dbInstance
}

// Connect opens a Go-native SQLite connection.
func (d *Database) Connect(path string) error {
	db, err := sql.Open("sqlite", path)
	if err != nil {
		return err
	}
	d.conn = db
	log.Printf("OmniData Go: Successfully connected to %s", path)
	return nil
}

// ExecuteQuery runs a SQL command and returns the results as a Go map slice.
func (d *Database) ExecuteQuery(query string) ([]map[string]interface{}, error) {
	rows, err := d.conn.Query(query)
	if err != nil {
		return nil, err
	}
	defer rows.Close()

	cols, _ := rows.Columns()
	results := make([]map[string]interface{}, 0)

	for rows.Next() {
		columns := make([]interface{}, len(cols))
		columnPointers := make([]interface{}, len(cols))
		for i := range columns {
			columnPointers[i] = &columns[i]
		}

		if err := rows.Scan(columnPointers...); err != nil {
			return nil, err
		}

		m := make(map[string]interface{})
		for i, colName := range cols {
			val := columnPointers[i].(*interface{})
			m[colName] = *val
		}
		results = append(results, m)
	}

	return results, nil
}
