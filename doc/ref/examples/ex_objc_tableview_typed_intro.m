/* @@Example: ex_objc_tableview_typed_intro @@ */

#import <Tightdb/Tightdb.h>

/* Defines a new table with two columns Name and Age. */

TIGHTDB_TABLE_2(PeopleTable,
                Name, String,
                Age, Int);

int main()
{
    @autoreleasepool {

        /* Creates a new table of the type defined above. */

        PeopleTable *table = [[PeopleTable alloc] init];

        /* Adds rows to the table. */

        PeopleTable_Cursor *cursor = [table addEmptyRow];
        cursor.Name = @"Brian";
        cursor.Age = 10;

        cursor = [table addEmptyRow];
        cursor.Name = @"Sofie";
        cursor.Age = 40;

        cursor = [table addEmptyRow];
        cursor.Name = @"Sam";
        cursor.Age = 76;

        /* Place the result of a query in a table view. */

        PeopleTable_View *tableView = [[[table where].Age columnIsGreaterThan:20] findAll];

        /* Itereato over the result in the table view. */

        for (PeopleTable_Cursor *curser in tableView) {
            NSLog(@"This person is over the age of 20: %@", [curser Name]);
        }



    }
}

/* @@EndExample@@ */
