SET AUTOCOMMIT ON;
CREATE TABLE TicTacToe (
    userId INT PRIMARY KEY,
    username VARCHAR(30) NOT NULL,
    pass VARCHAR(10) NOT NULL,
    win INT DEFAULT(0),
    loss INT DEFAULT(0),
    draw INT DEFAULT(0),
    CONSTRAINT win_CHK CHECK (win >= 0),
    CONSTRAINT loss_CHK CHECK (loss >= 0),
    CONSTRAINT draw_CHK CHECK (draw >= 0)
);
INSERT ALL
    INTO TICTACTOE VALUES(1,'rayyan', 'Ahmed@1', 0,0,0)
    INTO TICTACTOE VALUES(2,'rafay', 'Ghani@2', 0,0,0)
    INTO TICTACTOE VALUES(3,'riyya', 'Ahmed@3', 0,0,0)
    INTO TICTACTOE VALUES(4,'romeesa', 'Ahmed@4', 0,0,0)
    SELECT * FROM dual;

INSERT INTO TICTACTOE values(5, 'raheel', 'ghani', null, null, null);

SELECT COUNT(*) FROM tictactoe;
drop table tictactoe;

