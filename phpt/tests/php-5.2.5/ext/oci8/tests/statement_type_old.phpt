--TEST--
ocistatementtype()
--SKIPIF--
<?php if (!extension_loaded('oci8')) die("skip no oci8 extension"); ?>
--FILE--
<?php

require dirname(__FILE__)."/connect.inc";

if (!empty($dbase)) {
	var_dump($c = ocilogon($user, $password, $dbase));
}
else {
	var_dump($c = ocilogon($user, $password));
}

$sqls = Array(
    "SELECT * FROM table",
    "DELETE FROM table WHERE id = 1",
    "INSERT INTO table VALUES(1)",
    "UPDATE table SET id = 1",
	"DROP TABLE table",
	"CREATE OR REPLACE PROCEDURE myproc(v1 NUMBER) as BEGIN DBMS_OUTPUT.PUT_LINE(v1); END;",
    "CREATE TABLE table (id NUMBER)",
    "ALTER TABLE table ADD (col1 NUMBER)",
    "BEGIN NULL; END;",
    "DECLARE myn NUMBER BEGIN myn := 1; END;",
    "CALL myproc(1)",
    "WRONG SYNTAX",
    ""
);

foreach ($sqls as $sql) {
	$s = ociparse($c, $sql);
	var_dump(ocistatementtype($s));
}

echo "Done\n";

?>
--EXPECTF--
resource(%d) of type (oci8 connection)
string(6) "SELECT"
string(6) "DELETE"
string(6) "INSERT"
string(6) "UPDATE"
string(4) "DROP"
string(6) "CREATE"
string(6) "CREATE"
string(5) "ALTER"
string(5) "BEGIN"
string(7) "DECLARE"
string(4) "CALL"
string(7) "UNKNOWN"
string(7) "UNKNOWN"
Done
