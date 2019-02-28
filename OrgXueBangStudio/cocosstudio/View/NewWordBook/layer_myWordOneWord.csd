<GameFile>
  <PropertyGroup Name="layer_myWordOneWord" Type="Layer" ID="ee0bc6ba-00c1-4ac5-be57-f18ea92b9a08" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" Tag="65" ctype="GameLayerObjectData">
        <Size X="1000.0000" Y="1334.0000" />
        <Children>
          <AbstractNodeData Name="panel_center" ActionTag="1511458742" Tag="138" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="147.5000" RightMargin="147.5000" TopMargin="264.5000" BottomMargin="264.5000" ClipAble="False" ColorAngle="90.0000" LeftEage="243" RightEage="243" TopEage="283" BottomEage="283" Scale9OriginX="-243" Scale9OriginY="-283" Scale9Width="486" Scale9Height="566" ctype="PanelObjectData">
            <Size X="705.0000" Y="805.0000" />
            <Children>
              <AbstractNodeData Name="oneWordbg" Visible="False" ActionTag="-145853754" VisibleForFrame="False" Tag="2117" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="-13.0000" RightMargin="-13.0000" TopMargin="-14.0000" BottomMargin="-14.0000" ctype="SpriteObjectData">
                <Size X="731.0000" Y="833.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="352.5000" Y="402.5000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="1.0369" Y="1.0348" />
                <FileData Type="Normal" Path="Image/BigImg/oneWordbg.png" Plist="" />
                <BlendFunc Src="1" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="btn_sound" ActionTag="113810600" Tag="1311" IconVisible="False" LeftMargin="532.0000" RightMargin="73.0000" TopMargin="4.0000" BottomMargin="725.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="70" Scale9Height="54" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="100.0000" Y="76.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="582.0000" Y="763.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.8255" Y="0.9478" />
                <PreSize X="0.1418" Y="0.0944" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="MarkedSubImage" Path="Image/SubImg/Commom/CommNewWord/newWordGame_icon_sound.png" Plist="Image/SubTexture/Commom/CommNewWord.plist" />
                <PressedFileData Type="MarkedSubImage" Path="Image/SubImg/Commom/CommNewWord/newWordGame_icon_sound.png" Plist="Image/SubTexture/Commom/CommNewWord.plist" />
                <NormalFileData Type="MarkedSubImage" Path="Image/SubImg/Commom/CommNewWord/newWordGame_icon_sound.png" Plist="Image/SubTexture/Commom/CommNewWord.plist" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="txt_word" ActionTag="312973573" Tag="1344" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="131.8350" RightMargin="516.1650" TopMargin="133.4140" BottomMargin="592.5860" FontSize="60" LabelText="..." VerticalAlignmentType="VT_Center" OutlineSize="3" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="57.0000" Y="79.0000" />
                <AnchorPoint ScaleY="0.5000" />
                <Position X="131.8350" Y="632.0860" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="59" G="130" B="155" />
                <PrePosition X="0.1870" Y="0.7852" />
                <PreSize X="0.0809" Y="0.0981" />
                <FontResource Type="Normal" Path="Font/WDTTianniu.ttf" Plist="" />
                <OutlineColor A="255" R="115" G="0" B="76" />
                <ShadowColor A="255" R="115" G="0" B="76" />
              </AbstractNodeData>
              <AbstractNodeData Name="scrollView_info" ActionTag="189043936" Tag="1312" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="133.0750" RightMargin="111.9250" TopMargin="241.6230" BottomMargin="163.3770" TouchEnable="True" ClipAble="True" BackColorAlpha="102" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ScrollDirectionType="Vertical" ctype="ScrollViewObjectData">
                <Size X="460.0000" Y="400.0000" />
                <Children>
                  <AbstractNodeData Name="txt_word_desc" ActionTag="-1167244710" Tag="1345" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" IsCustomSize="True" FontSize="36" LabelText="..." ShadowOffsetX="1.0000" ShadowOffsetY="-1.0000" ctype="TextObjectData">
                    <Size X="460.0000" Y="400.0000" />
                    <AnchorPoint />
                    <Position />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="0" G="0" B="0" />
                    <PrePosition />
                    <PreSize X="1.0000" Y="1.0000" />
                    <FontResource Type="Default" Path="" Plist="" />
                    <OutlineColor A="255" R="104" G="65" B="0" />
                    <ShadowColor A="255" R="241" G="176" B="66" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="363.0750" Y="363.3770" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5150" Y="0.4514" />
                <PreSize X="0.6525" Y="0.4969" />
                <SingleColor A="255" R="255" G="150" B="100" />
                <FirstColor A="255" R="255" G="150" B="100" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
                <InnerNodeSize Width="460" Height="400" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="500.0000" Y="667.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="0.7050" Y="0.6034" />
            <SingleColor A="255" R="255" G="255" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>