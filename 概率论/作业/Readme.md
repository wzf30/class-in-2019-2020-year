1. 音频格式问题：   
    (1)pcm（pcm_s16le），wav，speex(speex-wb)，推荐使用pcm 
    (2)采样率为16000 或者 8000. 推荐使用16000，比特率为16bit 
    (3)单声道 
    (4)现可以通过ffmpeg的exe文件快速转码 ffmpeg -y -i test.mp3 -acodec pcm_s16le -f s16le -ac 1 -ar 16000 test.pcm，用现成Java代码未发现能符合要求的方式。 
    
    
    
2. 依赖：
    (1) 
    <dependency>
    <groupId>com.google.code.gson</groupId>
    <artifactId>gson</artifactId>
    <version>2.8.5</version>
    </dependency>
    (2)
    <dependency>
    <groupId>com.squareup.okio</groupId>
    <artifactId>okio</artifactId>
    <version>1.13.0</version>
    </dependency>
    (3)
    <dependency>
    <groupId>com.squareup.okhttp3</groupId>
    <artifactId>okhttp</artifactId>
    <version>3.8.1</version>
    </dependency>

    

3. 可以通过类WebIATWS的static方法public static double recognize(String File, String Accent, String ans)来得到分数，其中File表示转码文件的地址，accent表示方言的类别参数，ans表示要识别匹配的文本。
   一些参数:
   上海话  shanghainese    武汉话  wuhanese    台湾话  taiwanese
   南京话  nankinese       四川话  lmz         普通话  mandarin