#include <cstddef>
#include <cstdlib>
#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct TCell
{
  struct TCell             * m_Right;
  struct TCell             * m_Down;
  int                        m_Row;
  int                        m_Col;
  int                        m_Data;
} TCELL;

typedef struct TRowCol
{
  struct TRowCol           * m_Next;
  TCELL                    * m_Cells;
  int                        m_Idx;
} TROWCOL;

typedef struct TSparseMatrix
{
  TROWCOL                  * m_Rows;
  TROWCOL                  * m_Cols;
} TSPARSEMATRIX;
#endif /* __PROGTEST__ */

//Helpers

TROWCOL* getrowcol(TROWCOL* start, int id) {

    TROWCOL* itter = start;
    while (itter != nullptr) {

        if (itter->m_Idx == id)
            return itter;

        if (itter->m_Idx > id)
            return nullptr;

        itter = itter->m_Next;

    }
    return nullptr;
}

TCELL* getcell(TSPARSEMATRIX* m, int r, int c) {

    TCELL* out = nullptr;
    TROWCOL* r_itter = m->m_Rows;
    while(r_itter != nullptr) {

        if (r_itter->m_Idx == r) {
            TCELL* c_itter = r_itter->m_Cells;

            while (c_itter != nullptr) {

                if (c_itter->m_Col == c) {
                    out = c_itter;
                    break;
                }

                if (c_itter->m_Col > c)
                    break;

                c_itter = c_itter->m_Right;
            }
            break;
        }

        if (r_itter->m_Idx > r)
            break;

        r_itter  = r_itter->m_Next;
    }

    return out;
}

void removerowcol(TROWCOL **start, int idx) {
    if (*start == nullptr)
        return;

    TROWCOL *prev = nullptr, *itter = *start;

    do {


        if (itter->m_Idx ==  idx)  {
            if (prev != nullptr) prev->m_Next = itter->m_Next;
            else *start = itter->m_Next;
            
            free(itter);
            return;
        }

        prev = itter;
        if (itter != nullptr) itter = itter->m_Next;
    } while (itter != nullptr);
}

void insertrowcol(TROWCOL **start, TROWCOL *i) {
    if (*start == nullptr) {
        i->m_Next = nullptr;
        *start = i;
        return;
    }
    TROWCOL *prev = nullptr, *next = *start;

    do {
        if (next == nullptr  || next->m_Idx > i->m_Idx) {

            //Found the space, insert
            if (prev != nullptr) prev->m_Next = i;
            else *start = i;

            i->m_Next = next;
            return;
        }

        prev = next;
        if (next != nullptr) next = next->m_Next;
    } while (prev != nullptr);
}

void initMatrix(TSPARSEMATRIX *m) {

    if (!m)
        return;
    
    m->m_Rows = nullptr;
    m->m_Cols = nullptr;
}

void addSetCell(TSPARSEMATRIX  *m, int rowIdx, int colIdx, int data) {
    TCELL *c = getcell(m, rowIdx, colIdx);
    if (c == nullptr) {

        TCELL *new_c = (TCELL*)malloc(sizeof(TCELL));
        if (!new_c) return;

        new_c->m_Row = rowIdx;
        new_c->m_Col = colIdx;
        new_c->m_Data = data;

        //Find neighbors
    
        TROWCOL* col = getrowcol(m->m_Cols, colIdx);
        if (col == nullptr) {
            col = (TROWCOL*) malloc(sizeof(TROWCOL));
            col->m_Idx = colIdx;
            col->m_Cells = nullptr;
            insertrowcol(&(m->m_Cols), col);
        }

        TROWCOL* row = getrowcol(m->m_Rows, rowIdx);
        if (row == nullptr) {
            row = (TROWCOL*) malloc(sizeof(TROWCOL));
            row->m_Idx = rowIdx;
            row->m_Cells = nullptr;
            insertrowcol(&(m->m_Rows), row);
        }

        TCELL *rprev = nullptr, *rnext = row->m_Cells;
        TCELL *cprev = nullptr, *cnext = col->m_Cells;

        while (true) {
            if (rnext == nullptr)
                break;
            if (rnext->m_Col > colIdx)
                break;
            rprev = rnext;
            if (rnext != nullptr) rnext = rnext->m_Right;
        }

        while (true) {
            if (cnext == nullptr)
                break;
            if (cnext->m_Row > rowIdx)
                break;
            cprev = cnext;
            if (cnext != nullptr) cnext = cnext->m_Down;
        }

        if (rprev != nullptr) rprev->m_Right = new_c;
        else row->m_Cells = new_c;
        new_c->m_Right = rnext;

        if (cprev != nullptr) cprev->m_Down = new_c;
        else col->m_Cells = new_c;
        new_c->m_Down = cnext;

        return;
    }
    c->m_Data = data;
}

bool removeCell(TSPARSEMATRIX *m, int rowIdx, int colIdx) {

    //Quick workaround
    if (getcell(m, rowIdx, colIdx) == nullptr)
        return false;

    TROWCOL* r = getrowcol(m->m_Rows, rowIdx);
    TROWCOL* c = getrowcol(m->m_Cols, colIdx);
    if (c == nullptr || r == nullptr) return false;

    TCELL *rprev = nullptr, *ri = r->m_Cells;
    TCELL *cprev = nullptr, *ci = c->m_Cells;

    while (true) {
        if (ri == nullptr)
            return false;

        if (ri->m_Col == colIdx)
            break;
        rprev = ri;
        if (ri != nullptr) ri = ri->m_Right;
    }

    while (true) {
        if (ci == nullptr)
            return false;

        if (ci->m_Row == rowIdx)
            break;

        cprev = ci;
        if (ci != nullptr) ci = ci->m_Down;
    }

    if (rprev != nullptr) rprev->m_Right = ri->m_Right;
    else r->m_Cells = ri->m_Right;
    
    if (cprev != nullptr) cprev->m_Down = ci->m_Down;
    else c->m_Cells = ci->m_Down;

    //ri and ci should be same
    free(ri);

    //Check for row/col removal
    if (r->m_Cells == nullptr) removerowcol(&(m->m_Rows), r->m_Idx);
    if (c->m_Cells == nullptr) removerowcol(&(m->m_Cols), c->m_Idx);

    return true;
}

void freeMatrix(TSPARSEMATRIX *m) {
    TROWCOL* itter = m->m_Rows;
    while (itter != nullptr) {

        TCELL *ritter = itter->m_Cells;
        while (ritter != nullptr) {
            TCELL* toFree = ritter;
            ritter = ritter->m_Right;
            free(toFree);
        }
        itter = itter->m_Next;
    }

    itter = m->m_Rows;
    while (itter != nullptr) {
        TROWCOL* toFree = itter;
        itter = itter->m_Next;
        free(toFree);
    }

    itter = m->m_Cols;
    while (itter != nullptr) {
        TROWCOL* toFree = itter;
        itter = itter->m_Next;
        free(toFree);
    }

}

#ifndef __PROGTEST__

void printmat(TSPARSEMATRIX *m) {

    //Print rows with items
    TROWCOL* ritter = m->m_Rows;
    while (ritter != nullptr) {

        printf("ROW: %d; CELLS: ", ritter->m_Idx);

        TCELL* cellit = ritter->m_Cells;
        while (cellit != nullptr) {


            printf("[ r: %d, c: %d, d: %d ] ", cellit->m_Row, cellit->m_Col, cellit->m_Data);
            cellit = cellit->m_Right;
        }
        printf("\n");
        ritter  = ritter->m_Next;
    }

    printf("\n");

    //Print cols with items
    TROWCOL* citter = m->m_Cols;
    while (citter != nullptr) {

        printf("COL: %d; CELLS: ", citter->m_Idx);

        TCELL* cellit = citter->m_Cells;
        while (cellit != nullptr) {


            printf("[ r: %d, c: %d, d: %d] ", cellit->m_Row, cellit->m_Col, cellit->m_Data);
            cellit = cellit->m_Down;
        }
        printf("\n");
        citter  = citter->m_Next;
    }
}

int main ( int argc, char * argv [] )
{   
    TSPARSEMATRIX  m;
    initMatrix ( &m );
    addSetCell ( &m, 0, 1, 10 );
    addSetCell ( &m, 1, 0, 20 );
    addSetCell ( &m, 1, 5, 30 );
    addSetCell ( &m, 2, 1, 40 );
    assert ( m . m_Rows
             && m . m_Rows -> m_Idx == 0
             && m . m_Rows -> m_Cells
             && m . m_Rows -> m_Cells -> m_Row == 0
             && m . m_Rows -> m_Cells -> m_Col == 1
             && m . m_Rows -> m_Cells -> m_Data == 10
             && m . m_Rows -> m_Cells -> m_Right == nullptr );

    assert ( m . m_Rows -> m_Next
             && m . m_Rows -> m_Next -> m_Idx == 1
             && m . m_Rows -> m_Next -> m_Cells
             && m . m_Rows -> m_Next -> m_Cells -> m_Row == 1
             && m . m_Rows -> m_Next -> m_Cells -> m_Col == 0
             && m . m_Rows -> m_Next -> m_Cells -> m_Data == 20
             && m . m_Rows -> m_Next -> m_Cells -> m_Right
             && m . m_Rows -> m_Next -> m_Cells -> m_Right -> m_Row == 1
             && m . m_Rows -> m_Next -> m_Cells -> m_Right -> m_Col == 5
             && m . m_Rows -> m_Next -> m_Cells -> m_Right -> m_Data == 30
             && m . m_Rows -> m_Next -> m_Cells -> m_Right -> m_Right == nullptr );
    assert ( m . m_Rows -> m_Next -> m_Next
             && m . m_Rows -> m_Next -> m_Next -> m_Idx == 2
             && m . m_Rows -> m_Next -> m_Next -> m_Cells
             && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Row == 2
             && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Col == 1
             && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Data == 40
             && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Right == nullptr );
    assert ( m . m_Rows -> m_Next -> m_Next -> m_Next == nullptr );
    assert ( m . m_Cols
             && m . m_Cols -> m_Idx == 0
             && m . m_Cols -> m_Cells
             && m . m_Cols -> m_Cells -> m_Row == 1
             && m . m_Cols -> m_Cells -> m_Col == 0
             && m . m_Cols -> m_Cells -> m_Data == 20
             && m . m_Cols -> m_Cells -> m_Down == nullptr );
    assert ( m . m_Cols -> m_Next
             && m . m_Cols -> m_Next -> m_Idx == 1
             && m . m_Cols -> m_Next -> m_Cells
             && m . m_Cols -> m_Next -> m_Cells -> m_Row == 0
             && m . m_Cols -> m_Next -> m_Cells -> m_Col == 1
             && m . m_Cols -> m_Next -> m_Cells -> m_Data == 10
             && m . m_Cols -> m_Next -> m_Cells -> m_Down
             && m . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Row == 2
             && m . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Col == 1
             && m . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Data == 40
             && m . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Down == nullptr );
    assert ( m . m_Cols -> m_Next -> m_Next
             && m . m_Cols -> m_Next -> m_Next -> m_Idx == 5
             && m . m_Cols -> m_Next -> m_Next -> m_Cells
             && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Row == 1
             && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Col == 5
             && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Data == 30
             && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Down == nullptr );
    assert ( m . m_Cols -> m_Next -> m_Next -> m_Next == nullptr );
    assert ( m . m_Rows -> m_Cells == m . m_Cols -> m_Next -> m_Cells );
    assert ( m . m_Rows -> m_Next -> m_Cells == m . m_Cols -> m_Cells );
    assert ( m . m_Rows -> m_Next -> m_Cells -> m_Right == m . m_Cols -> m_Next -> m_Next -> m_Cells );
    assert ( m . m_Rows -> m_Next -> m_Next -> m_Cells == m . m_Cols -> m_Next -> m_Cells -> m_Down );
    addSetCell ( &m, 230, 190, 50 );
    //printmat(&m);
    assert ( m . m_Rows
             && m . m_Rows -> m_Idx == 0
             && m . m_Rows -> m_Cells
             && m . m_Rows -> m_Cells -> m_Row == 0
             && m . m_Rows -> m_Cells -> m_Col == 1
             && m . m_Rows -> m_Cells -> m_Data == 10
             && m . m_Rows -> m_Cells -> m_Right == nullptr );
    assert ( m . m_Rows -> m_Next
             && m . m_Rows -> m_Next -> m_Idx == 1
             && m . m_Rows -> m_Next -> m_Cells
             && m . m_Rows -> m_Next -> m_Cells -> m_Row == 1
             && m . m_Rows -> m_Next -> m_Cells -> m_Col == 0
             && m . m_Rows -> m_Next -> m_Cells -> m_Data == 20
             && m . m_Rows -> m_Next -> m_Cells -> m_Right
             && m . m_Rows -> m_Next -> m_Cells -> m_Right -> m_Row == 1
             && m . m_Rows -> m_Next -> m_Cells -> m_Right -> m_Col == 5
             && m . m_Rows -> m_Next -> m_Cells -> m_Right -> m_Data == 30
             && m . m_Rows -> m_Next -> m_Cells -> m_Right -> m_Right == nullptr );
    assert ( m . m_Rows -> m_Next -> m_Next
             && m . m_Rows -> m_Next -> m_Next -> m_Idx == 2
             && m . m_Rows -> m_Next -> m_Next -> m_Cells
             && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Row == 2
             && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Col == 1
             && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Data == 40
             && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Right == nullptr );
    assert ( m . m_Rows -> m_Next -> m_Next -> m_Next
             && m . m_Rows -> m_Next -> m_Next -> m_Next -> m_Idx == 230
             && m . m_Rows -> m_Next -> m_Next -> m_Next -> m_Cells
             && m . m_Rows -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Row == 230
             && m . m_Rows -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Col == 190
             && m . m_Rows -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Data == 50
             && m . m_Rows -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Right == nullptr );
    assert ( m . m_Rows -> m_Next -> m_Next -> m_Next -> m_Next == nullptr );
    assert ( m . m_Cols
             && m . m_Cols -> m_Idx == 0
             && m . m_Cols -> m_Cells
             && m . m_Cols -> m_Cells -> m_Row == 1
             && m . m_Cols -> m_Cells -> m_Col == 0
             && m . m_Cols -> m_Cells -> m_Data == 20
             && m . m_Cols -> m_Cells -> m_Down == nullptr );
    assert ( m . m_Cols -> m_Next
             && m . m_Cols -> m_Next -> m_Idx == 1
             && m . m_Cols -> m_Next -> m_Cells
             && m . m_Cols -> m_Next -> m_Cells -> m_Row == 0
             && m . m_Cols -> m_Next -> m_Cells -> m_Col == 1
             && m . m_Cols -> m_Next -> m_Cells -> m_Data == 10
             && m . m_Cols -> m_Next -> m_Cells -> m_Down
             && m . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Row == 2
             && m . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Col == 1
             && m . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Data == 40
             && m . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Down == nullptr );
    assert ( m . m_Cols -> m_Next -> m_Next
             && m . m_Cols -> m_Next -> m_Next -> m_Idx == 5
             && m . m_Cols -> m_Next -> m_Next -> m_Cells
             && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Row == 1
             && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Col == 5
             && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Data == 30
             && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Down == nullptr );
    assert ( m . m_Cols -> m_Next -> m_Next -> m_Next
             && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Idx == 190
             && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells
             && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Row == 230
             && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Col == 190
             && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Data == 50
             && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Down == nullptr );
    assert ( m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Next == nullptr );
    assert ( m . m_Rows -> m_Cells == m . m_Cols -> m_Next -> m_Cells );
    assert ( m . m_Rows -> m_Next -> m_Cells == m . m_Cols -> m_Cells );
    assert ( m . m_Rows -> m_Next -> m_Cells -> m_Right == m . m_Cols -> m_Next -> m_Next -> m_Cells );
    assert ( m . m_Rows -> m_Next -> m_Next -> m_Cells == m . m_Cols -> m_Next -> m_Cells -> m_Down );
    assert ( m . m_Rows -> m_Next -> m_Next -> m_Next -> m_Cells == m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells );
    assert ( removeCell ( &m, 0, 1 ) );
    assert ( !removeCell ( &m, 0, 1 ) );
    assert ( !removeCell ( &m, 1, 2 ) );
    assert ( m . m_Rows
             && m . m_Rows -> m_Idx == 1
             && m . m_Rows -> m_Cells
             && m . m_Rows -> m_Cells -> m_Row == 1
             && m . m_Rows -> m_Cells -> m_Col == 0
             && m . m_Rows -> m_Cells -> m_Data == 20
             && m . m_Rows -> m_Cells -> m_Right
             && m . m_Rows -> m_Cells -> m_Right -> m_Row == 1
             && m . m_Rows -> m_Cells -> m_Right -> m_Col == 5
             && m . m_Rows -> m_Cells -> m_Right -> m_Data == 30
             && m . m_Rows -> m_Cells -> m_Right -> m_Right == nullptr );
    assert ( m . m_Rows -> m_Next
             && m . m_Rows -> m_Next -> m_Idx == 2
             && m . m_Rows -> m_Next -> m_Cells
             && m . m_Rows -> m_Next -> m_Cells -> m_Row == 2
             && m . m_Rows -> m_Next -> m_Cells -> m_Col == 1
             && m . m_Rows -> m_Next -> m_Cells -> m_Data == 40
             && m . m_Rows -> m_Next -> m_Cells -> m_Right == nullptr );
    assert ( m . m_Rows -> m_Next -> m_Next
             && m . m_Rows -> m_Next -> m_Next -> m_Idx == 230
             && m . m_Rows -> m_Next -> m_Next -> m_Cells
             && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Row == 230
             && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Col == 190
             && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Data == 50
             && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Right == nullptr );
    assert ( m . m_Rows -> m_Next -> m_Next -> m_Next == nullptr );
    assert ( m . m_Cols
             && m . m_Cols -> m_Idx == 0
             && m . m_Cols -> m_Cells
             && m . m_Cols -> m_Cells -> m_Row == 1
             && m . m_Cols -> m_Cells -> m_Col == 0
             && m . m_Cols -> m_Cells -> m_Data == 20
             && m . m_Cols -> m_Cells -> m_Down == nullptr );
    assert ( m . m_Cols -> m_Next
             && m . m_Cols -> m_Next -> m_Idx == 1
             && m . m_Cols -> m_Next -> m_Cells
             && m . m_Cols -> m_Next -> m_Cells -> m_Row == 2
             && m . m_Cols -> m_Next -> m_Cells -> m_Col == 1
             && m . m_Cols -> m_Next -> m_Cells -> m_Data == 40
             && m . m_Cols -> m_Next -> m_Cells -> m_Down == nullptr );
    assert ( m . m_Cols -> m_Next -> m_Next
             && m . m_Cols -> m_Next -> m_Next -> m_Idx == 5
             && m . m_Cols -> m_Next -> m_Next -> m_Cells
             && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Row == 1
             && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Col == 5
             && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Data == 30
             && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Down == nullptr );
    assert ( m . m_Cols -> m_Next -> m_Next -> m_Next
             && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Idx == 190
             && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells
             && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Row == 230
             && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Col == 190
             && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Data == 50
             && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Down == nullptr );
    assert ( m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Next == nullptr );
    assert ( m . m_Rows -> m_Cells == m . m_Cols -> m_Cells );
    assert ( m . m_Rows -> m_Cells -> m_Right == m . m_Cols -> m_Next -> m_Next -> m_Cells );
    assert ( m . m_Rows -> m_Next -> m_Cells == m . m_Cols -> m_Next -> m_Cells );
    assert ( m . m_Rows -> m_Next -> m_Next -> m_Cells == m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells );

    freeMatrix ( &m );

    initMatrix(&m);
    addSetCell(&m, 25, 25, 1);

    addSetCell(&m, 26, 25, 1);

    addSetCell(&m, 27, 25, 1);
    printmat(&m);

    removeCell(&m, 25, 25);
    //printmat(&m);

    printf("%d\n", removeCell(&m, 27, 25));
    printmat(&m);

    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
