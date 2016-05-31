//
//  SecondViewController.swift
//  ios
//
//  Created by SINO on 2016. 5. 21..
//  Copyright © 2016년 soma. All rights reserved.
//

import UIKit

class SecondViewController: UIViewController {

    @IBOutlet weak var webView: UIWebView!
    
    @IBAction func back(sender: AnyObject) {
        if webView.canGoBack {
            webView.goBack()
        }
    }
    @IBAction func forward(sender: AnyObject) {
        if webView.canGoForward {
            webView.goForward()
        }
    }
    @IBAction func stop(sender: AnyObject) {
        webView.stopLoading()
    }
    @IBAction func refresh(sender: AnyObject) {
        webView.reload()
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
        webView.loadRequest(NSURLRequest(URL: NSURL(string: "http://www.google.com")!))
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
}

