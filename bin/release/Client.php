<?php
header( 'content-type: text/html; charset=utf-8' );
class Client {
   private $adress,$port;
   function __construct($adress,$port){
        $this->adress = $adress;
        $this->port = $port;
   }


   private  function  send($msg){
        $socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
        $response = '';
        if ($socket !== false) {
            $result = socket_connect($socket, $this->adress, $this->port);
            if ($result !== false) {
                $in = json_encode($msg);
                socket_write($socket, $in, strlen($in));
                $out = '';
               /* while ($out = socket_read($socket, 2048)) {
                   var_dump($out);
                   $response.= $out;
                }*/
            }
            socket_close($socket);
        }
        return $response;
   }
   public  function execute($program,$arguments = ''){
        $this->send(
            array('responses'=>
              array(
                array(
                  'type'=>'execute',
                  'program' => $program,
                )
              )
            )
          );
   }
   public  function speak($msg){
        $this->send(
           array('responses'=>
              array(
                array(
                  'type'=>'talk',
                  'sentence' => utf8_encode($msg)
                )
              )
            )
          );
    }
    public  function sound($file){
        $this->send(
           array('responses'=>
              array(
                array(
                  'type'=>'sound',
                  'file' => $file
                )
              )
            )
          );
    }
}

$yanaWindow = new Client('127.0.0.1',11000);
$yanaWindow->speak('Test vocal sous l\'environnement actuel!');
/*sleep(2);
$yanaWindow->sound('cris.wav');
sleep(2);
$yanaWindow->speak('Test anal');
sleep(2);
$yanaWindow->sound('pet.wav');
sleep(2);
$yanaWindow->speak('Test applicatif');
sleep(2);
$yanaWindow->execute('"D:\Programmes\Photoshop CS3\PhotoshopPortable.exe"');
*/

?>

