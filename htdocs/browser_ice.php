<?php

file_put_contents( 'e:/signaling/browser_ice', $_POST['q'] );
return;

$handle = fopen( 'e:/signaling/browser_ice' , 'w' );
fwrite( $handle, $POST['q'] );
fclose( $handle );
?>