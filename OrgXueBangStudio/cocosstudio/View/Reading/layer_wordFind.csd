<GameFile>
  <PropertyGroup Name="layer_wordFind" Type="Layer" ID="2a79853f-276d-4fe4-b30b-0b600c4b98b2" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" Tag="148" ctype="GameLayerObjectData">
        <Size X="750.0000" Y="1000.0000" />
        <Children>
          <AbstractNodeData Name="panel_top" ActionTag="-1791850570" Tag="149" IconVisible="False" PercentWidthEnable="True" PercentWidthEnabled="True" HorizontalEdge="BothEdge" VerticalEdge="TopEdge" BottomMargin="800.0000" TouchEnable="True" ClipAble="False" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="750.0000" Y="200.0000" />
            <Children>
              <AbstractNodeData Name="img_back" ActionTag="816858457" Tag="30" IconVisible="False" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentWidthEnabled="True" BottomMargin="-1800.0000" Scale9Enable="True" TopEage="212" BottomEage="29" Scale9OriginY="212" Scale9Width="750" Scale9Height="24" ctype="ImageViewObjectData">
                <Size X="750.0000" Y="2000.0000" />
                <Children>
                  <AbstractNodeData Name="img_notfind" ActionTag="-1102197636" VisibleForFrame="False" Tag="72" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="110.0000" RightMargin="110.0000" TopMargin="1076.5000" BottomMargin="876.5000" LeftEage="174" RightEage="174" TopEage="15" BottomEage="15" Scale9OriginX="174" Scale9OriginY="15" Scale9Width="182" Scale9Height="17" ctype="ImageViewObjectData">
                    <Size X="530.0000" Y="47.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="375.0000" Y="900.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.4500" />
                    <PreSize X="0.7067" Y="0.0235" />
                    <FileData Type="MarkedSubImage" Path="Image/SubImg/Reading/word_notfind.png" Plist="Image/SubTexture/Reading.plist" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleY="1.0000" />
                <Position Y="200.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition Y="1.0000" />
                <PreSize X="1.0000" Y="10.0000" />
                <FileData Type="MarkedSubImage" Path="Image/SubImg/Reading/word_back.png" Plist="Image/SubTexture/Reading.plist" />
              </AbstractNodeData>
              <AbstractNodeData Name="panel_close" ActionTag="1080750902" Tag="289" IconVisible="False" LeftMargin="17.0000" RightMargin="613.0000" TopMargin="26.0000" BottomMargin="54.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="120.0000" Y="120.0000" />
                <Children>
                  <AbstractNodeData Name="img_close" ActionTag="1343741898" Tag="29" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="39.5000" RightMargin="39.5000" TopMargin="39.5000" BottomMargin="39.5000" LeftEage="14" RightEage="14" TopEage="14" BottomEage="14" Scale9OriginX="14" Scale9OriginY="14" Scale9Width="13" Scale9Height="13" ctype="ImageViewObjectData">
                    <Size X="41.0000" Y="41.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="60.0000" Y="60.0000" />
                    <Scale ScaleX="1.5000" ScaleY="1.5000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.3417" Y="0.3417" />
                    <FileData Type="MarkedSubImage" Path="Image/SubImg/Reading/btn_close.png" Plist="Image/SubTexture/Reading.plist" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint />
                <Position X="17.0000" Y="54.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.0227" Y="0.2700" />
                <PreSize X="0.1600" Y="0.6000" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleY="1.0000" />
            <Position Y="1000.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition Y="1.0000" />
            <PreSize X="1.0000" Y="0.2000" />
            <SingleColor A="255" R="150" G="200" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="scroll_word" ActionTag="-141779163" Tag="150" IconVisible="False" PercentWidthEnable="True" PercentWidthEnabled="True" TopMargin="200.0000" TouchEnable="True" ClipAble="True" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ScrollDirectionType="Vertical" ctype="ScrollViewObjectData">
            <Size X="750.0000" Y="800.0000" />
            <Children>
              <AbstractNodeData Name="panel_wordBook" ActionTag="1835223459" Tag="153" IconVisible="False" PercentWidthEnable="True" PercentWidthEnabled="True" LeftMargin="0.0001" RightMargin="-0.0001" TopMargin="17.0120" BottomMargin="1082.9880" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="750.0000" Y="100.0000" />
                <Children>
                  <AbstractNodeData Name="text_word" ActionTag="-671221712" Tag="162" IconVisible="False" LeftMargin="56.4510" RightMargin="344.5490" TopMargin="18.1377" BottomMargin="15.8623" FontSize="48" LabelText="lightweight" OutlineSize="3" OutlineEnabled="True" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ShadowEnabled="True" ctype="TextObjectData">
                    <Size X="349.0000" Y="66.0000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="56.4510" Y="48.8623" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="236" G="244" B="27" />
                    <PrePosition X="0.0753" Y="0.4886" />
                    <PreSize X="0.4653" Y="0.6600" />
                    <FontResource Type="Normal" Path="Font/gillsansultrabold.ttf" Plist="" />
                    <OutlineColor A="255" R="115" G="0" B="76" />
                    <ShadowColor A="255" R="115" G="0" B="76" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="img_addNewWord" ActionTag="232872352" Tag="31" IconVisible="False" LeftMargin="492.5000" RightMargin="42.5000" TopMargin="16.5000" BottomMargin="18.5000" TouchEnable="True" LeftEage="80" RightEage="80" TopEage="25" BottomEage="25" Scale9OriginX="80" Scale9OriginY="25" Scale9Width="55" Scale9Height="15" ctype="ImageViewObjectData">
                    <Size X="215.0000" Y="65.0000" />
                    <Children>
                      <AbstractNodeData Name="Image_4" ActionTag="-369033993" Tag="32" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="8.0000" RightMargin="8.0000" TopMargin="9.7500" BottomMargin="16.2500" LeftEage="47" RightEage="47" TopEage="7" BottomEage="7" Scale9OriginX="47" Scale9OriginY="7" Scale9Width="105" Scale9Height="25" ctype="ImageViewObjectData">
                        <Size X="199.0000" Y="39.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="107.5000" Y="35.7500" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.5500" />
                        <PreSize X="0.9256" Y="0.6000" />
                        <FileData Type="MarkedSubImage" Path="Image/SubImg/Reading/word_add.png" Plist="Image/SubTexture/Reading.plist" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="600.0000" Y="51.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.8000" Y="0.5100" />
                    <PreSize X="0.2867" Y="0.6500" />
                    <FileData Type="MarkedSubImage" Path="Image/SubImg/Reading/word_addback.png" Plist="Image/SubTexture/Reading.plist" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="img_newBook" ActionTag="-1491923441" VisibleForFrame="False" Tag="106" IconVisible="False" LeftMargin="539.5000" RightMargin="89.5000" TopMargin="17.0000" BottomMargin="19.0000" TouchEnable="True" LeftEage="39" RightEage="39" TopEage="21" BottomEage="21" Scale9OriginX="39" Scale9OriginY="21" Scale9Width="43" Scale9Height="22" ctype="ImageViewObjectData">
                    <Size X="121.0000" Y="64.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="600.0000" Y="51.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.8000" Y="0.5100" />
                    <PreSize X="0.1613" Y="0.6400" />
                    <FileData Type="MarkedSubImage" Path="Image/SubImg/Reading/word_newBook.png" Plist="Image/SubTexture/Reading.plist" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleY="1.0000" />
                <Position X="0.0001" Y="1182.9880" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition Y="0.9858" />
                <PreSize X="1.0000" Y="0.0833" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="panel_img" ActionTag="1118460235" Tag="158" IconVisible="False" PercentWidthEnable="True" PercentWidthEnabled="True" TopMargin="122.7181" BottomMargin="677.2819" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="750.0000" Y="400.0000" />
                <Children>
                  <AbstractNodeData Name="img_word" ActionTag="-1988487723" Tag="164" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="75.0000" RightMargin="75.0000" LeftEage="66" RightEage="66" TopEage="66" BottomEage="66" Scale9OriginX="66" Scale9OriginY="66" Scale9Width="68" Scale9Height="68" ctype="ImageViewObjectData">
                    <Size X="600.0000" Y="400.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="375.0000" Y="200.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.8000" Y="1.0000" />
                    <FileData Type="MarkedSubImage" Path="Image/SubImg/Commom/CommBG/imgload.png" Plist="Image/SubTexture/Commom/CommBG.plist" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleY="1.0000" />
                <Position Y="1077.2819" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition Y="0.8977" />
                <PreSize X="1.0000" Y="0.3333" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="panel_sound" ActionTag="803560398" Tag="29" IconVisible="False" PercentWidthEnable="True" PercentWidthEnabled="True" TopMargin="500.0000" BottomMargin="571.0000" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="750.0000" Y="129.0000" />
                <Children>
                  <AbstractNodeData Name="img_play" ActionTag="669601785" Tag="30" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="332.0000" RightMargin="332.0000" TopMargin="26.0000" BottomMargin="26.0000" TouchEnable="True" LeftEage="9" RightEage="9" TopEage="9" BottomEage="9" Scale9OriginX="9" Scale9OriginY="9" Scale9Width="68" Scale9Height="59" ctype="ImageViewObjectData">
                    <Size X="86.0000" Y="77.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="375.0000" Y="64.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.1147" Y="0.5969" />
                    <FileData Type="MarkedSubImage" Path="Image/SubImg/Reading/btn_com_audio.png" Plist="Image/SubTexture/Reading.plist" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleY="1.0000" />
                <Position Y="700.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition Y="0.5833" />
                <PreSize X="1.0000" Y="0.1075" />
                <SingleColor A="255" R="255" G="165" B="0" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="panel_english" ActionTag="1301714512" Tag="159" IconVisible="False" PercentWidthEnable="True" PercentWidthEnabled="True" LeftMargin="0.0015" RightMargin="-0.0015" TopMargin="600.0000" BottomMargin="350.0000" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="750.0000" Y="250.0000" />
                <Children>
                  <AbstractNodeData Name="text_enExplain" ActionTag="1921407669" Tag="165" IconVisible="False" LeftMargin="60.0000" RightMargin="50.0000" TopMargin="150.0000" IsCustomSize="True" FontSize="36" LabelText="" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="640.0000" Y="100.0000" />
                    <AnchorPoint />
                    <Position X="60.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="0" G="0" B="0" />
                    <PrePosition X="0.0800" />
                    <PreSize X="0.8533" Y="0.4000" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleY="1.0000" />
                <Position X="0.0015" Y="600.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition Y="0.5000" />
                <PreSize X="1.0000" Y="0.2083" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="panel_showBtn" ActionTag="1631969908" Tag="160" IconVisible="False" PercentWidthEnable="True" PercentWidthEnabled="True" LeftMargin="0.0007" RightMargin="-0.0007" TopMargin="800.0000" BottomMargin="300.0000" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="750.0000" Y="100.0000" />
                <Children>
                  <AbstractNodeData Name="img_showCn" ActionTag="-1118039426" Tag="33" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="265.5000" RightMargin="265.5000" TopMargin="14.0000" BottomMargin="14.0000" TouchEnable="True" LeftEage="72" RightEage="72" TopEage="23" BottomEage="23" Scale9OriginX="72" Scale9OriginY="23" Scale9Width="75" Scale9Height="26" ctype="ImageViewObjectData">
                    <Size X="219.0000" Y="72.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="375.0000" Y="50.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.2920" Y="0.7200" />
                    <FileData Type="MarkedSubImage" Path="Image/SubImg/Reading/word_cn.png" Plist="Image/SubTexture/Reading.plist" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleY="1.0000" />
                <Position X="0.0007" Y="400.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition Y="0.3333" />
                <PreSize X="1.0000" Y="0.0833" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="panel_chinese" ActionTag="663833141" Tag="161" IconVisible="False" PercentWidthEnable="True" PercentWidthEnabled="True" LeftMargin="0.0011" RightMargin="-0.0011" TopMargin="796.4066" BottomMargin="103.5934" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="750.0000" Y="300.0000" />
                <Children>
                  <AbstractNodeData Name="text_cnExplain" ActionTag="-96045370" Tag="167" IconVisible="False" LeftMargin="60.0000" RightMargin="50.0000" TopMargin="100.0000" IsCustomSize="True" FontSize="36" LabelText="" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="640.0000" Y="200.0000" />
                    <AnchorPoint />
                    <Position X="60.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="0" G="0" B="0" />
                    <PrePosition X="0.0800" />
                    <PreSize X="0.8533" Y="0.6667" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleY="1.0000" />
                <Position X="0.0011" Y="403.5934" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition Y="0.3363" />
                <PreSize X="1.0000" Y="0.2500" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint />
            <Position />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="1.0000" Y="0.8000" />
            <SingleColor A="255" R="255" G="255" B="255" />
            <FirstColor A="255" R="255" G="150" B="100" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
            <InnerNodeSize Width="750" Height="1200" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>