#　優先度制御通信プログラムver1.00

##　プログラム概要
* TCPベースのファイルアップロードプログラムです
* 設定の変更でディレクトリごとの優先度制御と同一優先度の送信スケジューリングが行われます
* 同一優先度のIDはラウンドロビン方式，同一IDのデータはFIFOで送信順を決定します(デフォルト設定)

##　動作環境
* C++11以上
* GCC 4.8以上
* Linux環境(ファイル検索にLinuxのコマンド機能を利用)

##　ディレクトリ構成
* このプログラムは以下のような構成になっています
* 指定がない限りsrc/内をホームディレクトリとします
```
├─docs/
└─src/
  　├─conf/
  　├─log/
  　├─script/
  　├─source/
  　├─makefile
  　├─start_priority_client.sh
  　└─start_priority_server.sh
```
* conf/ : 設定ファイル用ディレクトリ
* log/ : ログ保存用ディレクトリ
* script/ : 動作補助用スクリプト
* sourve/ : プログラムソースコード

## 使用方法
##### 1. 設定ファイルの変更
##### サーバ用設定
* 優先度設定ファイル(conf/server_prioritydata.csv)の変更
優先度設定ファイルはカンマ区切りで以下を表します
優先度制御通信プログラムで取り扱うファイルごとに以下を設定します
```
deviceid,dataid,savedir,priority
```
* deviceid : ファイルを送信するクライアントのデバイスID(1~254)
* dataid : ファイルのID(1~254，異なるデバイスID間では重複可能)
* savedir : ファイルの保存先(末尾は/)
* priority : ファイル送信の優先度で小さいほど優先的に通信される(1~4)
```
例
1,1,/from_device1/pri1/,1
1,2,/from_device1/pri2/,2
2,1,/from_device2/pri3/,3
2,2,/from_device2/pri4/,4
```
* conf/config.hの以下を環境に合わせて変更
```
#define PORT 11111(サーバのポート番号)
#define SERVER_IP "111.111.111.111" //サーバのIPアドレス
#define MEASURE_THROUGHPUT false //スループット計測を行うか否か(trueで計測)
#define UPLOAD_DB false　//データベースとの連携を行うか否か(trueで連携)
#define RECVBUFSIZE 3000 //サーバのクライアント１台あたりのバッファサイズ
```

##### クライアント用設定
* 優先度設定ファイル(conf/client_prioritydata.csv)の変更
優先度設定ファイルはカンマ区切りで以下を表します
優先度制御通信プログラムで取り扱うファイルごとに以下を設定します
```
dataid,readdir,priority
```
* dataid : ファイルのID(1~254)
* readdir : ファイルの保存先(末尾は/)
* priority : ファイル送信の優先度で小さいほど優先的に通信される(1~4)
```
例
1,/device1/pri1/,1
2,/device1/pri2/,2
```
* conf/config.hの以下を環境に合わせて変更
```
#define PORT 11111(サーバのポート番号)
#define SERVER_IP "1111.111.111.111" //サーバのIPアドレス
#define DEVICE_ID 1 //クライアントのデバイスID
#define SENDBUFSIZE 3000 //バッファサイズ(最大65535)
```

#### 2. プログラムのビルド
#####2.1.```make clean```の実行
#####2.2.makefileの変更
* source/makefile以下の行を変更してください
```
TARGET_S = サーバプログラム名(半角英数字推奨)
TARGET_C = クライアントプログラム名(半角英数字推奨)
```

#####2.3.ビルドしたいプログラムのオプションを添えて実行
* ```make server```サーバをビルド
* ```make client```クライアントをビルド
* ```make all```両方をビルド

##### 3. プログラムの実行(定期実行)
* src内で特定のシェルを実行(server:```start_priority_server.sh```client:```start_priority_client.sh```)
* 一度の実行で接続切れか送信ファイルがなくなるまで通信される

## 設定ファイルについて

## ビルドについて

## データベースとの連携について
* サーバ側の設定により受信したファイルをデータベースに登録できます
* 動作環境
 * PostgreSQL環境
 * サーバプログラム動作環境でpsqlコマンドが実行可能

#### 使用データベースの設定について
* データベース設定ファイル(conf/dbconfig.h)を変更
```
#define TABLE_NAME "tablename" //使用テーブル名
#define DB_SVNAME "222.222.222.222" //DBサーバ名(IPアドレス)
#define DB_SVPORT "22222" //DBサーバ接続ポート
#define DB_USERNAME "user_name" //DBユーザ名
#define DB_NAME "db_name" //DB名
#define DB_PASSWORD "password" //DBのパスワード
```

#### テーブル作成について
* 以下のクエリでテーブル作成をしてください
```
CREATE TABLE tablename
(
  id serial NOT NULL,
  time timestamp without time zone,
  file_timestamp timestamp without time zone,
  dataid real,
  deviceid real,
  filesize real,
  filepath text,
  CONSTRAINT DBNAME_pkey PRIMARY KEY (id)
);
```

## 追加設定項目
* ソースコードを変更することで動作の変更が可能

* コネクション制御のON/OFF
```
agent->setAgentOpt(FUNC_CONNECTION,1); //デフォルト設定
```
* 第二引数が0:コネクション制御を有効化
第二引数が1:コネクション制御を無効化

* 詳細な送信順制御の指定
```
agent->setMode(FILO_RR); //デフォルト設定
```
* 引数にはFIFO_RR，FILO_RR，FIFO，FILO，SPTFを指定可能
* FIFO_RRではIDごとラウンドロビン方式で送信対象とし，新しいファイルが優先される


