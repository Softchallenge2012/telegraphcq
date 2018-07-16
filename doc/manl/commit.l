.\\" auto-generated by docbook2man-spec $Revision: 1.25 $
.TH "COMMIT" "l" "2002-11-22" "SQL - Language Statements" "SQL Commands"
.SH NAME
COMMIT \- commit the current transaction
.SH SYNOPSIS
.sp
.nf
COMMIT [ WORK | TRANSACTION ]
  
.sp
.fi
.SS "INPUTS"
.PP
.TP
\fBWORK\fR
.TP
\fBTRANSACTION\fR
Optional keywords. They have no effect.
.PP
.SS "OUTPUTS"
.PP
.TP
\fBCOMMIT\fR
Message returned if the transaction is successfully committed.
.TP
\fBWARNING: COMMIT: no transaction in progress\fR
If there is no transaction in progress.
.PP
.SH "DESCRIPTION"
.PP
\fBCOMMIT\fR commits the current transaction. All
changes made by the transaction become visible to others
and are guaranteed to be durable if a crash occurs.
.SS "NOTES"
.PP
The keywords WORK and TRANSACTION are noise and can be omitted.
.PP
Use ROLLBACK [\fBrollback\fR(l)]
to abort a transaction.
.SH "USAGE"
.PP
To make all changes permanent:
.sp
.nf
COMMIT WORK;
   
.sp
.fi
.SH "COMPATIBILITY"
.SS "SQL92"
.PP
SQL92 only specifies the two forms COMMIT
and COMMIT WORK. Otherwise full compatibility.