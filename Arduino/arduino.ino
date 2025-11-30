#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

// 设置OneNet参数
const char* mqttServer = "mqtts.heclouds.com"; // OneNET的MQTT服务器地址
const uint16_t mqttPort = 1883;               // OneNET的MQTT端口号
const char *Device_ID = "替换为设备ID";         // 设备ID
const char *Product_ID = "替换为产品ID ";              // 产品ID
const char *Api_KEY = "token生成工具生成"; // 鉴权信息

// 设置WiFi参数
const char *WiFi_SSID = "wifi名称";                // WiFi名称
const char *WiFi_Password = "wifi密码";       // WiFi密码

// 创建对象
WiFiClient espClient;
PubSubClient client(espClient);

// 全局变量定义
float wave1[198];  // 波形数据1
float wave2[198];  // 波形数据2

// 声明函数
void WiFi_Init(void);
void MQTT_Init(void);
void MQTT_Reconnection(void);
void MQTT_Callback(char *MQTT_Topic, uint8_t *MQTT_Payload, uint16_t MQTT_Payload_Len);
void initWaveformData(void);
void uploadWaveformData(void);

void setup()//setup只执行一次，联网放这里面就行
{
  Serial.begin(115200);//串口初始化
  WiFi_Init();         //WiFi初始化
  MQTT_Init();         //MQTT初始化
  
  // 初始化波形数据
  initWaveformData();
}

void loop()//loop相当于其他单片机的while(1)
{
  // 判断MQTT连接了没,没的话重新连接
  if (!client.connected())
    MQTT_Reconnection();

  // 上传波形数据
  uploadWaveformData();

  client.loop();
  delay(5000);  // 每隔5秒上传一次数据
}

// 初始化波形数据
void initWaveformData(void)
{
  // 自定义wave1数据（模拟正弦波+噪声）
  for(int i = 0; i < 198; i++) {
    // 基础正弦波
    float baseValue = sin(i * 2 * PI / 50.0);
    // 添加高频噪声
    float noise = (random(0, 200) - 100) * 0.001;
    // 生成最终电压值 (0.00V - 0.20V范围)
    wave1[i] = 0.10 + baseValue * 0.08 + noise;
  }
  
  // 自定义wave2数据（模拟方波+噪声）
  for(int i = 0; i < 198; i++) {
    // 方波信号（每25个点切换）
    float squareWave = (i / 25) % 2 == 0 ? 0.15 : 0.05;
    // 添加噪声
    float noise = (random(0, 100) - 50) * 0.001;
    wave2[i] = squareWave + noise;
  }
  
  Serial.println("✅ 波形数据初始化完成");
  Serial.print("wave1[0-2]: ");
  Serial.print(wave1[0], 4); Serial.print("V, ");
  Serial.print(wave1[1], 4); Serial.print("V, ");
  Serial.print(wave1[2], 4); Serial.println("V");
  Serial.print("wave2[0-2]: ");
  Serial.print(wave2[0], 4); Serial.print("V, ");
  Serial.print(wave2[1], 4); Serial.print("V, ");
  Serial.print(wave2[2], 4); Serial.println("V");
}

// 上传波形数据
void uploadWaveformData(void)
{
  Serial.println("⬆️ 开始上传波形数据...");
  
  // 分批次上传wave1数据（wave1_0到wave1_9）
  for(int batch = 0; batch < 10; batch++) {
    String waveData = "";
    
    // 每批处理20个数据点（最后一批18个）
    int startIndex = batch * 20;
    int endIndex = min(startIndex + 20, 198);
    int pointCount = endIndex - startIndex;
    
    // 将数据点转换为字符串
    for(int i = startIndex; i < endIndex; i++) {
      int voltageInt = (int)(wave1[i] * 10000);
      char buffer[5];
      sprintf(buffer, "%04d", voltageInt);
      waveData += String(buffer);
    }
    
    // 构建变量名 wave1_0 到 wave1_9
    char varName[10];
    sprintf(varName, "wave1_%d", batch);
    
    // 构建要上传的数据
    char json[200];
    sprintf(json, "{\"id\":\"%d\",\"version\":\"1.0\",\"params\":{"
                  "\"%s\":{\"value\":\"%s\"}"
                  "}}", 
            batch, varName, waveData.c_str());

    // 上传数据
    String publishTopic = String("$sys/") + Product_ID + "/" + Device_ID + "/thing/property/post";
    if(client.publish(publishTopic.c_str(), (const uint8_t*)json, strlen(json))) {
      Serial.print("✅ ");
      Serial.print(varName);
      Serial.print(" 上传成功 (");
      Serial.print(pointCount);
      Serial.print("个点): ");
      Serial.println(waveData.substring(0, 8) + "...");
    } else {
      Serial.print("❌ ");
      Serial.print(varName);
      Serial.println(" 上传失败");
    }
    
    delay(500); // 批次间隔0.5秒
  }
  
  Serial.println("---");
  
  // 分批次上传wave2数据（wave2_0到wave2_9）
  for(int batch = 0; batch < 10; batch++) {
    String waveData = "";
    
    // 每批处理20个数据点（最后一批18个）
    int startIndex = batch * 20;
    int endIndex = min(startIndex + 20, 198);
    int pointCount = endIndex - startIndex;
    
    // 将数据点转换为字符串
    for(int i = startIndex; i < endIndex; i++) {
      int voltageInt = (int)(wave2[i] * 10000);
      char buffer[5];
      sprintf(buffer, "%04d", voltageInt);
      waveData += String(buffer);
    }
    
    // 构建变量名 wave2_0 到 wave2_9
    char varName[10];
    sprintf(varName, "wave2_%d", batch);
    
    // 构建要上传的数据
    char json[200];
    sprintf(json, "{\"id\":\"%d\",\"version\":\"1.0\",\"params\":{"
                  "\"%s\":{\"value\":\"%s\"}"
                  "}}", 
            batch + 10, varName, waveData.c_str());

    // 上传数据
    String publishTopic = String("$sys/") + Product_ID + "/" + Device_ID + "/thing/property/post";
    if(client.publish(publishTopic.c_str(), (const uint8_t*)json, strlen(json))) {
      Serial.print("✅ ");
      Serial.print(varName);
      Serial.print(" 上传成功 (");
      Serial.print(pointCount);
      Serial.print("个点): ");
      Serial.println(waveData.substring(0, 8) + "...");
    } else {
      Serial.print("❌ ");
      Serial.print(varName);
      Serial.println(" 上传失败");
    }
    
    delay(500); // 批次间隔0.5秒
  }
  
  Serial.println("====================");
}

// WiFi初始化
void WiFi_Init(void)
{
  Serial.print("\r\n\r\nConnecting to ");
  Serial.print(WiFi_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WiFi_SSID, WiFi_Password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(" .");
  }
  // 打印WiFi连接成功提示语和本机IP地址
  Serial.print("\r\nWiFi Connection Successful.\r\nIP address: ");
  Serial.println(WiFi.localIP());
}

// MQTT初始化
void MQTT_Init(void)
{
  client.setServer(mqttServer, mqttPort);
  client.setCallback(MQTT_Callback);
}

// 收到信息时,会调用这个函数
void MQTT_Callback(char *MQTT_Topic, uint8_t *MQTT_Payload, uint16_t MQTT_Payload_Len)
{
  Serial.printf("📨 Topic: %s[%d]:\r\n", MQTT_Topic, MQTT_Payload_Len);
  for (uint16_t i = 0; i < MQTT_Payload_Len; i++)
  {
    Serial.print((char)MQTT_Payload[i]);
  }
  Serial.println();
}

void MQTT_Reconnection(void)
{
  while (!client.connected())
  {
    Serial.println("MQTT Reconnection.");
    if (client.connect(Device_ID, Product_ID, Api_KEY)) {
      Serial.println("MQTT Connection Successful.");
    } else {
      Serial.print("Connect Failed, Error Code = ");
      Serial.print(client.state());
      Serial.println(", Try again in 5 seconds.");
      delay(5000);
    }
  }
}
