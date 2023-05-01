import 'dart:collection';

import 'package:flutter/material.dart';
import 'package:http/http.dart' as http;
import 'dart:convert' as convert;

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return const MaterialApp(
      title: 'Plant Watering System',
      home: MyHomePage(title: 'Plant Watering System'),
    );
  }
}

class MyHomePage extends StatefulWidget {
  const MyHomePage({super.key, required this.title});
  final String title;

  @override
  State<MyHomePage> createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  int _counter = 0;
  String deviceId = "e00fce68036ae2c13dad559f";
  //"e00fce688962f1fbca445fb8";
  TextEditingController _nameController = TextEditingController(text: "plant name");
  String _accessToken = "55864584825f771c81bc45b98a91b1300e72e269";
  
  @override
  void initState() {
    super.initState();
  }

  Widget plantCardWidget(String name, int idx) {
    return Card(
              child:
                Container(
                  padding: EdgeInsets.all(10),
                  constraints: BoxConstraints(minWidth: 200, minHeight: 200),
                  child: 
                    Column(
                      mainAxisAlignment: MainAxisAlignment.center,
                      crossAxisAlignment: CrossAxisAlignment.center,
                      children: [
                        Text(name),
                        SizedBox(height:10),
                        Image.asset('assets/placeholder.png', height: 100, width: 100,),
                        SizedBox(height:10),
                        ElevatedButton(                
                          onPressed: () {
                            startMotorRequest(idx);
                          },                         
                          child: Text("Water plant now")
                        ),
                        SizedBox(height:10),
                        Text("Set moisture threshold:"),
                        SizedBox(height:10),
                        Column(
                          mainAxisAlignment: MainAxisAlignment.center,
                          crossAxisAlignment: CrossAxisAlignment.center,
                          children: [
                            ElevatedButton(
                              onPressed: () {
                                // changeMoistureThreshold(idx, "low");
                                calibrate(idx, "dry");

                              },                           
                              child: Text("Calibrate Dry")
                            ),
                            SizedBox(height:10),
                            ElevatedButton(
                              onPressed: () {
                                // changeMoistureThreshold(idx, "low");
                                calibrate(idx, "med");

                              },                           
                              child: Text("Calibrate Medium")
                            ),
                            SizedBox(height:10),
                            ElevatedButton(
                              onPressed: () {
                                // changeMoistureThreshold(idx, "low");
                                calibrate(idx, "wet");
                              },                           
                              child: Text("Calibrate Wet")
                            ),
                          ],
                        )
                      ],
                    )
                )
            );
  }

  Future<void> startMotorRequest(int idx) async {
    print("entered startMotorRequest()");
    var url = Uri.parse('https://api.particle.io/v1/devices/' + deviceId + '/ManualRun');
    var formData = <String, String>{
      'access_token': _accessToken,
      'params': idx.toString(),
    };
    var response = await http.post(url, body: formData);
    if (response.statusCode == 200) {
      var jsonResponse = convert.jsonDecode(response.body) as Map<String, dynamic>;
      print(jsonResponse.toString());
      // var itemCount = jsonResponse['totalItems'];
      // print('Number of books about http: $itemCount.');
    } else {
      print('Request failed with status: ${response.statusCode}.');
    }
  }

  Future<void> calibrate(int idx, String level) async {
      print("entered calibrate()");
      var url = Uri.parse('https://api.particle.io/v1/devices/' + deviceId + '/Calibrate');
      var formData = <String, String>{
        'access_token': _accessToken,
        'params': '$idx/'+level, 
      };
      var response = await http.post(url, body: formData);
      if (response.statusCode == 200) {
        var jsonResponse = convert.jsonDecode(response.body) as Map<String, dynamic>;
        print(jsonResponse.toString());
        // var itemCount = jsonResponse['totalItems'];
        // print('Number of books about http: $itemCount.');
      } else {
        print('Request failed with status: ${response.statusCode}.');
      }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text(widget.title),
      ),
      body: SingleChildScrollView(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: <Widget>[
            plantCardWidget("plant name 1", 0),
            plantCardWidget("plant name 2", 1),                
          ],
        ),
      ),
    );
  }
}
