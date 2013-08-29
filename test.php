<?php
error_reporting(E_ALL);
define('ROOT_PATH', "./");
echo "taocf version:".TCF_VERSION."\n";
$commonConfig=Common::getConfig('common.php');
echo "read config file common.php,\napp name is:{$commonConfig['name']}\n";
Common::debugDump('输出调试变量内容',521,array('1','2','3'));
$baba = new jiji_baba();